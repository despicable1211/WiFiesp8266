
#include "dma1.h"
#include <string.h>

void Uart1_DMA_Rx(void);
void Uart1_DAM_Tx(void);
void Uart1_Start_DMA_Tx(uint32_t size);


u8 ReceiBuff1[BUF_SIZE1];  //���ݻ�����
u8 SendBuff1[BUF_SIZE1];  //���ݻ�����



 	    

//DMA��ʼ��

void dma1_init(void)
{
        u16 DMA1_MEM_LEN;                                       //����DMAÿ�����ݴ��͵ĳ���
        DMA_InitTypeDef DMA_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;

        RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����

        
/**************** Tx_DMA conifg ************************/
        
        DMA_Cmd(DMA1_Channel4, DISABLE);   // ��DMAͨ��
        DMA_DeInit(DMA1_Channel4);   //��DMA��ͨ��7�Ĵ�������Ϊȱʡֵ
        
	DMA1_MEM_LEN=BUF_SIZE1;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)(&USART1->DR);  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32) SendBuff1;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = DMA1_MEM_LEN;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
        DMA_ClearFlag(DMA1_FLAG_GL4);                                 // ���DMA���б�־
        DMA_Cmd(DMA1_Channel4, DISABLE); // �ر�DMA
        DMA_ITConfig(DMA1_Channel4, DMA_IT_TC, ENABLE);            // ��������DMAͨ���ж�
        
/**************** Rx_DMA conifg ************************/ 
        
        DMA_Cmd(DMA1_Channel5, DISABLE);   // ��DMAͨ��
        DMA_DeInit(DMA1_Channel5);   //��DMA��ͨ��7�Ĵ�������Ϊȱʡֵ
        
	DMA1_MEM_LEN=BUF_SIZE1;
	DMA_InitStructure.DMA_PeripheralBaseAddr =(u32)(&USART1->DR);  //DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr =(u32) ReceiBuff1;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = DMA1_MEM_LEN;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte; //���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //����������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
        DMA_ClearFlag(DMA1_FLAG_GL5);                                 // ���DMA���б�־
        DMA_Cmd(DMA1_Channel5, ENABLE); // ��DMA
        DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);            // ��������DMAͨ���ж�
        
        //DMA�����ж�����  
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel4_IRQn;  
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);   
        NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;  
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
        NVIC_Init(&NVIC_InitStructure);      

}


/*******DMA�����жϺ���*******/
void DMA1_Channel5_IRQHandler(void)
{
    int temp = 0;  
    DMA_Cmd(DMA1_Channel5, DISABLE);       // �ر�DMA ����ֹ����

    DMA_ClearFlag( DMA1_FLAG_GL5 );        // ��DMA��־λ

    temp = BUF_SIZE1 - DMA_GetCurrDataCounter(DMA1_Channel5); //��ý��յ����ֽ���
//    for(temp=0;temp<BUF_SIZE1;temp++)
//    {
//      printf("ReceiBuff1[%d]=%x\n",temp,ReceiBuff1[temp]);
//    }
    //uart1Write((uint8_t*)ReceiBuff1[temp],BUF_SIZE1);
    DMA1_Channel5->CNDTR = BUF_SIZE1;        //  ���¸�ֵ����ֵ��������ڵ��������ܽ��յ�������֡��Ŀ

    DMA_Cmd(DMA1_Channel5, ENABLE);        // DMA �������ȴ����ݡ�
}
/******DMA�����жϺ���*******/
void DMA1_Channel4_IRQHandler(void)

{

    if(DMA_GetITStatus(DMA1_FLAG_TC4))

    {

        Uart1_DAM_Tx();

    }

}

void Uart1_DAM_Tx(void)

{

    DMA_ClearFlag(DMA1_FLAG_GL4);         // �����־

    DMA_Cmd(DMA1_Channel4, DISABLE);   // �ر�DMAͨ��
}

/********************************* 
@brief     :    DMA���ͺ������������ݵ�WIFIģ��
@param buf :    ���ݵ�ַ
@param len :    ���ݳ���

@return    ����ȷ�������ݳ��ȣ����󷵻� -1��
********************************/

int uart1Write( uint8_t *buf, uint32_t len)

{
    int i;
    
    if(NULL==buf) return -1;
    
    for(i=0;i<len;i++)
    {
      SendBuff1[i] = buf[i];
//      if(i>=2&&buf[i]==0xff)
//        SendBuff1[i++]=0x55;
//      len++;
    }

    Uart1_Start_DMA_Tx( len );
    
    return len;
}

 

void Uart1_Start_DMA_Tx(uint32_t size)

{

    DMA1_Channel4->CNDTR = (uint32_t)size; // ����Ҫ���͵��ֽ���Ŀ
    //USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  // ��������DMA����
    //DMA_Cmd(DMA1_Channel4, DISABLE );  //�ر�USART1 TX DMA1 ��ָʾ��ͨ��      
    //DMA_SetCurrDataCounter(DMA1_Channel4,size);//DMAͨ����DMA����Ĵ�С
    //DMA_Cmd(DMA1_Channel4, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 
    DMA_Cmd(DMA1_Channel4, ENABLE);        //��ʼDMA����

}





















