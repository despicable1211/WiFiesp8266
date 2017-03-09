#ifndef __DMA1_H
#define	__DMA1_H	   
#include <stm32f10x.h>

							    					    
#define BUF_SIZE1 15  //���ݳ���

extern u8 ReceiBuff1[BUF_SIZE1];  //���ݻ�����
extern u8 SendBuff1[BUF_SIZE1];  //���ݻ�����

void dma1_init(void);
int32_t uart1Write(uint8_t *buf, uint32_t len);
		   
#endif
