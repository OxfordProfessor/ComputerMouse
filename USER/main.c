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
#include "train.h"
#include "OLED.h"
#include "pwm.h"

//任务优先级
#define START_TASK_PRIO			1
//任务堆栈大小	
#define START_STK_SIZE 			128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);


//LED0任务
//设置任务优先级
#define LED0_TASK_PRIO 			1
//任务堆栈大小
#define LED0_STK_SIZE			128
//任务句柄
TaskHandle_t Led0Task_Handler;
//led0任务
void led0_task(void *pvParameters);

//主任务
//设置任务优先级
#define MAIN_TASK_PRIO 			1
//任务堆栈大小
#define MAIN_STK_SIZE			512
//任务句柄
TaskHandle_t MAINTask_Handler;
//led0任务
void main_task(void *pvParameters);

extern LinkList L;    //用于存放路径的链表

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
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,             
                (const char*    )"led0_task",           
                (uint16_t       )LED0_STK_SIZE,        
                (void*          )NULL,                  
                (UBaseType_t    )LED0_TASK_PRIO,        
                (TaskHandle_t*  )&Led0Task_Handler);  
    //创建主任务
    xTaskCreate((TaskFunction_t )main_task,            //任务函数
                (const char*    )"main_task",          //任务名称
                (uint16_t       )MAIN_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )MAIN_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&MAINTask_Handler);   //任务句柄   
	  UsartPrintf(USART_DEBUG, " Task OK\r\n");				
    vTaskDelete(StartTask_Handler); 	//删除开始任务
    taskEXIT_CRITICAL();            	//退出临界区
}

//LED0任务
void led0_task(void *p_arg)
{
	while(1)
	{
		LED3 = !LED3;
		vTaskDelay(500);		//延时500ms
	}
}

void main_task(void *p_arg)
{
	u8 key;           		 //保存键值
	while(1)
	{
 re:ShowStr(0,1,"1.Debug");
		ShowStr(2,1,"2.训练模式");
		ShowStr(4,1,"3.main");
		ShowStr(4,1,"4.测试内存");
		key=KEY_Scan(0);		//得到键值
		if(key)
		{						   
			switch(key)
			{				 
				case KEY0_PRES:		//Debug模式	
					ClearScreen();    //清屏
				  while(1)
					{
							ShowStr(0,1,"1.测试步长");
							ShowStr(2,1,"2.左转90");
							ShowStr(4,1,"3.右转90");
						  ShowStr(6,1,"返回");
							key = KEY_Scan(0);
						 	if(key)
							{						   
								switch(key)
								{	
									case KEY0_PRES:		
										Go_Onestep();
									break;
									case KEY1_PRES:		
										turn_left(90);
									break;
									case KEY2_PRES:		
										turn_right(90);
									break;
									case WKUP_PRES:		
										goto re;
									break;
								}
							}
					}
				break;
				case KEY1_PRES:		//训练模式
					while(!findpath());   //找到路径后跳出
				  tract_storage(&L);    //路线存入flash
					LED1 = ~LED1;       //灯变亮，说明最短路径已经找到并存入flash;
				break;
				case KEY2_PRES:		//主程序模式
					tract_follow();      //取出路线，放入链表
				  Traverse_List(&L);   //遍历链表，按照路线执行动作
				break;	
				case WKUP_PRES:
					printf("the size of p->data is %d",sizeof(L->data));
				break;
			}
		}
		else
		{
			 vTaskDelay(10); 
		}
	}
}

