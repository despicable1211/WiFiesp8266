
#include "dma.h"
#include <string.h>


u8 ReceiBuff[BUF_SIZE];  //���ݻ�����
u8 SendBuff[BUF_SIZE*2];  //���ݻ�����



u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 	    

//DMA��ʼ��

void dma_init(void)
{
        DMA_InitTypeDef DMA_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
        
/**************** Tx_DMA conifg ************************/
        
        DMA_Cmd(DMA1_Channel7, DISABLE);   // ��DMAͨ��
        DMA_DeInit(DMA1_Channel7);   //��DMA��ͨ��7�Ĵ�������Ϊȱʡֵ
        
	DMA1_MEM_LEN=BUF_SIZE;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)(&USART2->DR);  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32) SendBuff;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = DMA1_MEM_LEN;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
        DMA_ClearFlag(DMA1_FLAG_GL7);                                 // ���DMA���б�־
        DMA_Cmd(DMA1_Channel7, DISABLE); // �ر�DMA
        DMA_ITConfig(DMA1_Channel7, DMA_IT_TC, ENABLE);            // ��������DMAͨ���ж�
        
/**************** Rx_DMA conifg ************************/ 
        
        DMA_Cmd(DMA1_Channel6, DISABLE);   // ��DMAͨ��
        DMA_DeInit(DMA1_Channel6);   //��DMA��ͨ��7�Ĵ�������Ϊȱʡֵ
        
	DMA1_MEM_LEN=BUF_SIZE;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)(&USART2->DR);  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32) ReceiBuff;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = DMA1_MEM_LEN;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel6, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
        DMA_ClearFlag(DMA1_FLAG_GL6);                                 // ���DMA���б�־
        DMA_Cmd(DMA1_Channel6, ENABLE); // ��DMA
        DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);            // ��������DMAͨ���ж�
        
        //DMA�����ж�����  
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;  
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure); 
        
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;  
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure); 
}


/*******DMA�����жϺ���*******/
void DMA1_Channel6_IRQHandler(void)
{
 
    DMA_Cmd(DMA1_Channel6, DISABLE);       // �ر�DMA ����ֹ����

    DMA_ClearFlag( DMA1_FLAG_GL6 );        // ��DMA��־λ

    //temp = BUF_SIZE - DMA_GetCurrDataCounter(DMA1_Channel6); //��ý��յ����ֽ���
    
    DMA1_Channel6->CNDTR = BUF_SIZE;        //  ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ

    DMA_Cmd(DMA1_Channel6, ENABLE);        // DMA �������ȴ����ݡ�
}

/******DMA�����жϺ���*******/
void DMA1_Channel7_IRQHandler(void)

{

    if(DMA_GetITStatus(DMA1_FLAG_TC7))

    {

    DMA_ClearFlag(DMA1_FLAG_GL7);         // �����־

    DMA_Cmd(DMA1_Channel7, DISABLE);   // �ر�DMAͨ��   
    }
}

/********************************* 
@brief     :    DMA���ͺ������������ݵ�WIFIģ��
@param buf :    ���ݵ�ַ
@param len :    ���ݳ���

@return    ����ȷ�������ݳ��ȣ����󷵻� -1��
********************************/

int32_t uartWrite( uint8_t *buf, uint32_t len)

{
    int i;
    
    if(NULL==buf) return -1;
    
    for(i=0;i<len;i++)
    {
      SendBuff[i] = buf[i];
      if(i>=2&&buf[i]==0xff)
      { 
        SendBuff[i++]=0x55;
        len++;
      }      
    }
    
    DMA1_Channel7->CNDTR = len; // ����Ҫ���͵��ֽ���Ŀ

    DMA_Cmd(DMA1_Channel7, ENABLE);        //��ʼDMA����
    printf("MCU to WIFI:");
    for(i=0;i<len;i++)
    {
      printf("%x",SendBuff[i]);
    }
    printf("\n");
    return len;
}




















