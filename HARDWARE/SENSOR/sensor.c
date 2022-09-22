#include "sensor.h"

/*
  �����ഫ������ʼ�������ļ�
*/

#define  M    2
volatile float AD_Value[M];

void ADC_init()
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_InitTypeDef       ADC_InitStructure;
	DMA_InitTypeDef  DMA_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA1ʱ��ʹ��

	//ģ�������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;//PC0,PC1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//ģ������
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;//����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ�� 

	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	
	
	//DMA����  
    //DMAͨ������  
    DMA_DeInit(DMA2_Stream0);  
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;   
    //�����ַ  
    DMA_InitStructure.DMA_PeripheralBaseAddr = ((u32)ADC1+0x4c);  //(uint32_t)(&ADC1->DR)
    //�ڴ��ַ  
    DMA_InitStructure.DMA_Memory0BaseAddr =(u32)&AD_Value;  //DMA�ڴ����ַ  
    //dma���䷽��  
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  
    //����DMA�ڴ���ʱ�������ĳ���  
    DMA_InitStructure.DMA_BufferSize = M;  //DMAͨ����DMA����Ĵ�С	M*N
    //����DMA���������ģʽ��һ������  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  
    //����DMA���ڴ����ģʽ  
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  
    //���������ֳ�  
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  
    //�ڴ������ֳ�  
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;  
    //����DMA�Ĵ���ģʽ  
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  
    //����DMA�����ȼ���  
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
      
    //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������:��ʹ��FIFOģʽ    
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      
    //ָ����FIFO��ֵˮƽ  
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;          
    //ָ����Burstת�������ڴ洫��   
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         
    //ָ����Burstת��������Χת��  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   
      
    //����DMA��ͨ��           
    DMA_Init(DMA2_Stream0, &DMA_InitStructure); 
    //ʹ��ͨ��  
    DMA_Cmd(DMA2_Stream0, ENABLE);  	
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); //ʹ��ADC1ʱ��
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);    
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE); 
	//��ʼ��ADC  
    ADC_DeInit();  

	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
	ADC_CommonInitStructure.ADC_DMAAccessMode =  ADC_DMAAccessMode_Disabled; //DMAʧ�ܣ�//�ر�DMA��ȡ  
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 	
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  //���β������  
	ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
	ADC_InitStructure.ADC_ScanConvMode = ENABLE;//ɨ��ģʽ	
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//����ת��
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������� 
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
	ADC_InitStructure.ADC_NbrOfConversion = M;//1��ת���ڹ��������� Ҳ����ֻת����������1 
	ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��
	
	//����ģʽͨ������ 
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11,1, ADC_SampleTime_56Cycles);
	ADC_RegularChannelConfig(ADC1,ADC_Channel_12,2, ADC_SampleTime_56Cycles);
	
	//ʹ��ADC��DMA 
	ADC_DMARequestAfterLastTransferCmd(ADC1,ENABLE); //Դ���ݱ仯ʱ����DMA����
	ADC_DMACmd(ADC1,ENABLE);//ʹ��ADC��DMA����
	 
	//ʹ��ADC1 
	ADC_Cmd(ADC1, ENABLE);//����ADת����	
	
	//����ADC1�����ת��
	ADC_SoftwareStartConv(ADC1);
}