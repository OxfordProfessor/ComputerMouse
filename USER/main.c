#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "sram.h"
#include "malloc.h"
#include "led.h"
#include "timer.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "key.h"
#include "limits.h"
#include "beep.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


//任务优先级
#define START_TASK_PRIO			1
//任务堆栈大小	
#define START_STK_SIZE 			128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//TOUCH任务
//设置任务优先级
#define TOUCH_TASK_PRIO			2
//任务堆栈大小
#define TOUCH_STK_SIZE			128
//任务句柄
TaskHandle_t TouchTask_Handler;
//touch任务
void touch_task(void *pvParameters);

//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO 			3
//任务堆栈大小
#define LED0_STK_SIZE			128
//任务句柄
TaskHandle_t Led0Task_Handler;
//led0任务
void led0_task(void *pvParameters);

//EMWINDEMO任务
//设置任务优先级
#define EMWINDEMO_TASK_PRIO		4
//任务堆栈大小
#define EMWINDEMO_STK_SIZE		512
//任务句柄
TaskHandle_t EmwindemoTask_Handler;
//emwindemo_task任务
void emwindemo_task(void *pvParameters);

//收集任务
//设置任务优先级
#define COLLECT_TASK_PRIO		5
//任务堆栈大小
#define COLLECT_STK_SIZE		128
//任务句柄
TaskHandle_t CollectTask_Handler;
//collect_task任务
void collect_task(void *pvParameters);

//NET任务
//设置任务优先级
#define NET_TASK_PRIO		5
//任务堆栈大小
#define NET_STK_SIZE		128
//任务句柄
TaskHandle_t NetTask_Handler;
//net_task任务
void net_task(void *pvParameters);

//收集任务
//设置任务优先级
#define MINORCONTROL_TASK_PRIO		5
//任务堆栈大小
#define MINORCONTROL_STK_SIZE		128
//任务句柄
TaskHandle_t MinorControl_Handler;
//minorcontrol_task任务
void minorcontrol_task(void *pvParameters);

//回到岸边任务
//设置任务优先级
#define BACK_TASK_PRIO		6
//任务堆栈大小
#define BACK_STK_SIZE		128
//任务句柄
TaskHandle_t Back_Handler;
//minorcontrol_task任务
void back_task(void *pvParameters);

#define MSG_Q_NUM    1  		//传递消息队列的数量 
QueueHandle_t Msg_Queue;   		//按键值消息队列句柄

typedef struct
{
	int time;
	int value;
}DIRECTION;

typedef struct
{
	int time;
	int value;
}COMEBACK;

const char *devSubTopic[] = {"/gvrxJiLWkq4/Stm32Internet/user/get"};	//订阅主题
const char devPubTopic[] = "/gvrxJiLWkq4/Stm32Internet/user/update";	//发布主题

unsigned short timeCount = 0;	//发送间隔变量
unsigned short timeCount_date = 0;	//发送间隔变量
unsigned char *dataPtr = NULL;  //esp8266正常运作检查指针

extern char PUB_BUF1[256];
int main(void)
{
	delay_init(168);       	//延时初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 	//中断分组配置
	usart1_init(115200);    //串口1波特率设置(电脑调试接口)
	usart3_init(115200);    //串口3波特率设置(Esp8266通信串口)
	LED_Init();   			//LED初始化
	FSMC_SRAM_Init(); 		//SRAM初始化	
	mem_init(SRAMIN); 		//内部RAM初始化
	mem_init(SRAMEX); 		//外部RAM初始化
	mem_init(SRAMCCM);		//CCM初始化
	UsartPrintf(USART_DEBUG, " Hardware init OK\r\n");
	BEEP = 1;//鸣叫提示接入成功
	delay_ms(250);
	BEEP = 0;	
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄                
    vTaskStartScheduler();          //开启任务调度
}

//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	//创建队列
	Msg_Queue=xQueueCreate(MSG_Q_NUM,sizeof(int));        //创建队列Msg_Queue
	//创建触摸任务
    xTaskCreate((TaskFunction_t )touch_task,             
                (const char*    )"touch_task",           
                (uint16_t       )TOUCH_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )TOUCH_TASK_PRIO,        
                (TaskHandle_t*  )&TouchTask_Handler);   	
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,             
                (const char*    )"led0_task",           
                (uint16_t       )LED0_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LED0_TASK_PRIO,        
                (TaskHandle_t*  )&Led0Task_Handler);  
    //创建EMWIN Demo任务
    xTaskCreate((TaskFunction_t )emwindemo_task,             
                (const char*    )"emwindemo_task",           
                (uint16_t       )EMWINDEMO_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )EMWINDEMO_TASK_PRIO,        
                (TaskHandle_t*  )&EmwindemoTask_Handler);
	//创建Collect任务
    xTaskCreate((TaskFunction_t )collect_task,             
                (const char*    )"collect_task",           
                (uint16_t       )COLLECT_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )COLLECT_TASK_PRIO,        
                (TaskHandle_t*  )&CollectTask_Handler);  
    //创建Net任务
    xTaskCreate((TaskFunction_t )net_task,             
                (const char*    )"net_task",           
                (uint16_t       )NET_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )NET_TASK_PRIO,        
                (TaskHandle_t*  )&NetTask_Handler);   			
    //创建MinorControl任务
    xTaskCreate((TaskFunction_t )minorcontrol_task,             
                (const char*    )"minorcontrol_task",           
                (uint16_t       )MINORCONTROL_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )MINORCONTROL_TASK_PRIO,        
                (TaskHandle_t*  )& MinorControl_Handler);   
    //创建Back任务
    xTaskCreate((TaskFunction_t )back_task,             
                (const char*    )"back_task",           
                (uint16_t       )BACK_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )BACK_TASK_PRIO,        
                (TaskHandle_t*  )& Back_Handler); 
	vTaskSuspend(Back_Handler); 		//返回岸边任务，非必要不启动
	vTaskSuspend(CollectTask_Handler);  //暂停收集任务（主任务），中控台启动时才开始执行，否则不启动
	UsartPrintf(USART_DEBUG, " Task OK\r\n");				
    vTaskDelete(StartTask_Handler); 	//删除开始任务
    taskEXIT_CRITICAL();            	//退出临界区
}

//EMWINDEMO任务
void emwindemo_task(void *pvParameters)
{

}

//触摸任务的任务函数
void touch_task(void *pvParameters)
{

}

//LED0任务
void led0_task(void *p_arg)
{
	while(1)
	{
		LED0 = !LED0;
		vTaskDelay(500);		//延时500ms
	}
}
//收集任务
void collect_task(void *pvParameters)
{

}
//物联网控制函数，向服务器发送数据、接收互联网数据控制外设
void net_task(void *pvParameters) 
{

}
//人为辅助控制任务
void minorcontrol_task(void *pvParameters)
{

}
//回到岸边任务
void back_task(void *pvParameters)
{

}
