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
//FreeRTOSʱ��ͳ�����õĽ��ļ�����
volatile unsigned long long FreeRTOSRunTimeTicks;

extern _Bool terminal_flag;   //�����յ��־
extern _Bool block_flag;      //��·������־
extern volatile float AD_Value[M];

void start_time()
{
	count_ms_0=0;
	count_s_0=0;
	TIM_Cmd(TIM2,ENABLE);	//ʹ�ܶ�ʱ��3
}

//��ȡ��ʱ  ˵��������ϵͳ��ʱ����λ����
u16 mseconds()
{
	return count_ms_0;
}

//��ʼ��TIM4ʹ��ΪFreeRTOS��ʱ��ͳ���ṩʱ��
void ConfigureTimeForRunTimeStats(void)
{
	//��ʱ��4��ʼ������ʱ��ʱ��Ϊ84M����Ƶϵ��Ϊ84-1�����Զ�ʱ��3��Ƶ��
	//Ϊ84M/84=1M���Զ���װ��Ϊ50-1����ô��ʱ�����ھ���50us
	FreeRTOSRunTimeTicks=0;
	TIM4_Int_Init(50-1,84-1);	//��ʼ��TIM3
}

//ͨ�ö�ʱ��4�жϳ�ʼ��
//arr���Զ���װֵ��
//psc��ʱ��Ԥ��Ƶ��
//��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//Ft=��ʱ������Ƶ��,��λ:Mhz
//����ʹ�õ��Ƕ�ʱ��4!
void TIM4_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  ///ʹ��TIM3ʱ��
	
	TIM_TimeBaseInitStructure.TIM_Period = arr; 	//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  //��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseInitStructure);//��ʼ��TIM3
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE); //����ʱ��3�����ж�
	TIM_Cmd(TIM5,ENABLE); //ʹ�ܶ�ʱ��3
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM5_IRQn; //��ʱ��3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0x01; //��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0x00; //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

//��ʱ��3�жϷ�����
/*
	�������ƣ�·�߱�������
  �������ã���ȡflash�д洢��·����Ϣ��������ִ����Ӧ����
  �����������
  �����������Ӧ��������	
*/
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET) //����ж�
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
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);  //����жϱ�־λ
}

void TIM6_Int_Init(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);	//ʹ��TIM6ʱ��
	//��ʱ��TIM6��ʼ��
	TIM_TimeBaseInitStructure.TIM_Period = arr;			//�Զ���װ��ֵ
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;		//��ʱ����Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 

	TIM_TimeBaseInit(TIM6,&TIM_TimeBaseInitStructure);	//��ʼ��TIM6
	TIM_ITConfig(TIM6,TIM_IT_Update,ENABLE);			//����ʱ��6�����ж�
	TIM_Cmd(TIM6,ENABLE);								//ʹ�ܶ�ʱ��6
	NVIC_EnableIRQ(TIM6_DAC_IRQn );
	NVIC_InitStructure.NVIC_IRQChannel=TIM6_DAC_IRQn;		//��ʱ��6�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;	//�����ȼ�1
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

//��ʱ��6�жϷ�����
void TIM6_DAC_IRQHandler(void)    //���Ĵ����жϺ���д��TIM6_IRQHandler���������ǳ���ִ��
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)	//���TIM6�������
	{
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);		//���TIMx���±�־ 			
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);//ʹ��TIM7ʱ��
	//��ʱ��TIM7��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr;				//�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc;			//��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);		//��ʼ��TIM7
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE );			//����ʱ��7�����ж�
	TIM_Cmd(TIM7, ENABLE);								//ʹ�ܶ�ʱ��7
	//��ʼ��NVIC�Ĵ���
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;		//��ʱ��7�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//��ռ�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	//�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**************************************************************************
�������ܣ���TIM2,5��ʼ��Ϊ�������ӿ�ģʽ
��ڲ�������
����  ֵ����
**************************************************************************/
void Encoder_Init_TIM2(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);			

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;				//��©�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				//����
	GPIO_Init(GPIOA,&GPIO_InitStructure);						

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource0 ,GPIO_AF_TIM2);		//����ΪTIM2 �������ӿ�
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1 ,GPIO_AF_TIM2);		//����ΪTIM2 �������ӿ�
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;					//����Ƶ
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;		//�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���    
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//��ʼ����ʱ��

	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);						//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM2, ENABLE); 
}
void Encoder_Init_TIM5(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB, ENABLE);		
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);			//TIM5ʱ��ʹ��	84MHz

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;				//��©�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				//����
	GPIO_Init(GPIOA,&GPIO_InitStructure);						
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;				//���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//�ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;				//��©�������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;				//����
	GPIO_Init(GPIOB,&GPIO_InitStructure);						

	GPIO_PinAFConfig(GPIOA,GPIO_PinSource15 ,GPIO_AF_TIM5);		//����ΪTIM5 �������ӿ�
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3 ,GPIO_AF_TIM5);		//����ΪTIM5 �������ӿ�
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;					//����Ƶ
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;		//�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���    
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);				//��ʼ����ʱ��

	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 0;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);

	TIM_ClearFlag(TIM5, TIM_FLAG_Update);						//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM5, ENABLE); 
}
/**************************************************************************
�������ܣ���λʱ���ȡ����������
��ڲ�������ʱ��
����  ֵ���ٶ�ֵ
**************************************************************************/
int Encoder_TIM;
int Read_Encoder(u8 TIMX)
{    
	switch(TIMX)
	{
		case 2:  Encoder_TIM= (short)TIM2 -> CNT;  TIM2 -> CNT=0;break;	//��ȡ�����������ݲ�����
		case 5:  Encoder_TIM= (short)TIM5 -> CNT;  TIM5 -> CNT=0;break;	//��ȡ�����������ݲ�����
		default:  Encoder_TIM=0;
	}
	return Encoder_TIM;
}
/**************************************************************************
�������ܣ�TIM1�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM2_IRQHandler(void)
{ 		    		  			    
	if(TIM2->SR&0X0001)//����ж�
	{
		
	}				   
	TIM2->SR&=~(1<<0);//����жϱ�־λ 	    
}

/**************************************************************************
�������ܣ�TIM3�жϷ�����
��ڲ�������
����  ֵ����
**************************************************************************/
void TIM5_IRQHandler(void)
{ 		    		  			    
	if(TIM3->SR&0X0001)//����ж�
	{
		
	}				   
	TIM3->SR&=~(1<<0);//����жϱ�־λ 	    
}

/**************************************************************************
�������ܣ�PID�����ٶ�
��ڲ�����a	ʵ��ֵ	Target	Ŀ��ֵ
����  ֵ��PWM
**************************************************************************/
int Position_PID_Lift (int a,int Target)					//ʵ��ֵ��Ŀ��ֵ
{ 	
	float Position_KP=5.5,Position_KI=0.65,Position_KD=1;	//PI �̻�����    //80  0.4
	static float Bias,Pwm,Integral_bias,Last_Bias;
	Bias=a-Target;											//����ƫ��
	Integral_bias+=Bias;									//���ƫ��Ļ���
	Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);		//λ��ʽPID������
	Last_Bias=Bias;											//������һ��ƫ�� 
	return Pwm;												//�������
}

int Position_PID_Right (int a,int Target)					//ʵ��ֵ��Ŀ��ֵ
{ 	
	float Position_KP=5.5,Position_KI=0.65,Position_KD=1;		//PI �̻�����
	 static float Bias,Pwm,Integral_bias,Last_Bias;
	 Bias=a-Target;											//����ƫ��
	 Integral_bias+=Bias;									//���ƫ��Ļ���
	 Pwm=Position_KP*Bias+Position_KI*Integral_bias+Position_KD*(Bias-Last_Bias);		//λ��ʽPID������
	 Last_Bias=Bias;										//������һ��ƫ�� 
	 return Pwm;											//�������
}



//��ʱ��7�жϷ�����
void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)	//���TIM7�������
	{
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);		//���TIM7���±�־ 			
	}
	
	Encoder1=Read_Encoder(2);
	Encoder2=Read_Encoder(5);

	Motor_Left = (int)Position_PID_Lift (Encoder1,Target_Left);	//�ٶ�PI������
	
	if(Motor_Left > 1000)
		Motor_Left = 1000;
	else if(Motor_Left < -1000)
		Motor_Left = -1000;
	if(Target_Left==0)
		Motor_Left=0;
	Motor1(Motor_Left);

	Motor_Right =(int)Position_PID_Right (Encoder2,Target_Righ);	//�ٶ�PI������
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
