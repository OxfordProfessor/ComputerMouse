#include "timer.h"
#include "led.h"
#include "usart.h"
#include "pwm.h"

#define M 2

u16 count_ms_0=0;
u16 count_s_0=0;

int m,n;
int Motor_Left,Motor_Right;
int Target_Left,Target_Righ;

int Encoder1,Encoder2;

int Front,Left,Righ,Behind;
//FreeRTOS时间统计所用的节拍计数器
volatile unsigned long long FreeRTOSRunTimeTicks;

extern _Bool terminal_flag;   //到达终点标志
extern _Bool block_flag;      //道路阻塞标志
extern volatile float AD_Value[M];

void start_time()
{
	count_ms_0=0;
	count_s_0=0;
	TIM_Cmd(TIM2,ENABLE);	//使能定时器3
}

//获取计时  说明：返回系统计时，单位毫秒
u16 mseconds()
{
	return count_ms_0;
}

//初始化TIM4使其为FreeRTOS的时间统计提供时基
void ConfigureTimeForRunTimeStats(void)
{
	//定时器4初始化，定时器时钟为84M，分频系数为84-1，所以定时器3的频率
	//为84M/84=1M，自动重装载为50-1，那么定时器周期就是50us
	FreeRTOSRunTimeTicks=0;
	TIM4_Int_Init(50-1,84-1);	//初始化TIM3
}

//通用定时器4中断初始化
//arr：自动重装值。
//psc：时钟预分频数
//定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=定时器工作频率,单位:Mhz
//这里使用的是定时器4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///使能TIM3时钟
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//初始化TIM3
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //允许定时器3更新中断
	TIM_Cmd(TIM5,ENABLE); //使能定时器3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //定时器3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //子优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//定时器3中断服务函数
/*
	函数名称：路线遍历函数
  函数作用：读取flash中存储的路线信息到链表，并执行相应动作
  输入参数：无
  输出参数：相应动作函数	
*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //溢出中断
	{
		FreeRTOSRunTimeTicks++;
		if(AD_Value[0] < 1000)
		{
			block_flag = 1;
		}
		else
		{
			block_flag = 0;
		}
		if(AD_Value[1] > 2000)
		{
			terminal_flag = 1;
		}
		else
		{
			terminal_flag = 0;
		}
	}
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //清除中断标志位
}

void TIM6_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);	//使能TIM6时钟
	//定时器TIM6初始化
	TIM_TimeBaseInitStructure.TIM_Period = arr;			//自动重装载值
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;		//定时器分频
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);	//初始化TIM6
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);			//允许定时器6更新中断
	TIM_Cmd(TIM6,ENABLE);								//使能定时器6
	NVIC_EnableIRQ(TIM6_DAC_IRQn );
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;		//定时器6中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//子优先级1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

//定时器6中断服务函数
void TIM6_DAC_IRQHandler(void)    //犯的错误，中断函数写成TIM6_IRQHandler，不报错但是程序不执行
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)	//检查TIM6更新与否
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);		//清除TIMx更新标志 			
	}
	n++;
	if(n>200)
	{
		LED1=~LED1;
		LED2=~LED2;
		n = 0;
	}
}

void TIM7_Int_Init(u16 arr,u16 psc)
{
	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//使能TIM7时钟
	//定时器TIM7初始化
	TIM_TimeBaseStructure.TIM_Period = arr;				//自动重装载值
	TIM_TimeBaseStructure.TIM_Prescaler =psc;			//定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//向上计数模式
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);		//初始化TIM7
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );			//允许定时器7更新中断
	TIM_Cmd(TIM7, ENABLE);								//使能定时器7
	//初始化NVIC寄存器
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;		//定时器7中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//先占用优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//从优先级1级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************
函数功能：把TIM2,5初始化为编码器接口模式
入口参数：无
返回  值：无
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);			

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;				//开漏复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);						

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0 ,GPIO_AF_TIM2);		//复用为TIM2 编码器接口
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1 ,GPIO_AF_TIM2);		//复用为TIM2 编码器接口
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;					//不分频
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;		//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数    
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//初始化定时器

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);						//清除TIM的更新标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE); 
}
void Encoder_Init_TIM5(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);			//TIM5时钟使能	84MHz

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;				//开漏复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				//上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);						
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;				//开漏复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				//上拉
	GPIO_Init(GPIOB,&GPIO_InitStructure);						

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15 ,GPIO_AF_TIM5);		//复用为TIM5 编码器接口
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3 ,GPIO_AF_TIM5);		//复用为TIM5 编码器接口
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;					//不分频
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;		//设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数    
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);				//初始化定时器

	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM5, TIM_FLAG_Update);						//清除TIM的更新标志位
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE); 
}
/**************************************************************************
函数功能：单位时间读取编码器计数
入口参数：定时器
返回  值：速度值
**************************************************************************/
int Encoder_TIM;
int Read_Encoder(u8 TIMX)
{    
	switch(TIMX)
	{
		case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;	//读取编码器的数据并清零
		case 5:  Encoder_TIM= (short)TIM5 -> CNT;  TIM5 -> CNT=0;break;	//读取编码器的数据并清零
		default:  Encoder_TIM=0;
	}
	return Encoder_TIM;
}
/**************************************************************************
函数功能：TIM1中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//溢出中断
	{
		
	}				   
	TIM2->SR&=~(1<<0);//清除中断标志位 	    
}

/**************************************************************************
函数功能：TIM3中断服务函数
入口参数：无
返回  值：无
**************************************************************************/
void TIM5_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//溢出中断
	{
		
	}				   
	TIM3->SR&=~(1<<0);//清除中断标志位 	    
}

/**************************************************************************
函数功能：PID控制速度
入口参数：a	实际值	Target	目标值
返回  值：PWM
**************************************************************************/
int Position_PID_Lift (int a,int Target)					//实际值，目标值
{ 	
	float Position_KP=5.5,Position_KI=0.65,Position_KD=1;	//PI 固化参数    //80  0.4
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=a-Target;											//计算偏差
	Integral_bias+=Bias;									//求出偏差的积分
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);		//位置式PID控制器
	Last_Bias=Bias;											//保存上一次偏差 
	return Pwm;												//增量输出
}

int Position_PID_Right (int a,int Target)					//实际值，目标值
{ 	
	float Position_KP=5.5,Position_KI=0.65,Position_KD=1;		//PI 固化参数
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=a-Target;											//计算偏差
	 Integral_bias+=Bias;									//求出偏差的积分
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);		//位置式PID控制器
	 Last_Bias=Bias;										//保存上一次偏差 
	 return Pwm;											//增量输出
}



//定时器7中断服务函数
void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)	//检查TIM7更新与否
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);		//清除TIM7更新标志 			
	}
	
	Encoder1=Read_Encoder(2);
	Encoder2=Read_Encoder(5);

	Motor_Left = (int)Position_PID_Lift (Encoder1,Target_Left);	//速度PI控制器
	
	if(Motor_Left > 1000)
		Motor_Left = 1000;
	else if(Motor_Left < -1000)
		Motor_Left = -1000;
	if(Target_Left==0)
		Motor_Left=0;
	Motor1(Motor_Left);

	Motor_Right =(int)Position_PID_Right (Encoder2,Target_Righ);	//速度PI控制器
	if(Motor_Right > 1000)
		Motor_Right = 1000;
	else if(Motor_Right < -1000)
		Motor_Right = -1000;
	if(Target_Righ==0)
		Motor_Right=0;
	Motor2(Motor_Right);
			m++;
	if(m>200)
	{
		LED3=~LED3;
		LED4=~LED4;
		m = 0;
	}
}
