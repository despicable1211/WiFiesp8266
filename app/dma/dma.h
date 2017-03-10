#ifndef __DMA_H
#define	__DMA_H	   
#include <stm32f10x.h>

							    					    
#define BUF_SIZE 150  //���ݳ���

extern u8 ReceiBuff[BUF_SIZE];  //���ݻ�����
extern u8 SendBuff[BUF_SIZE*2];  //���ݻ�����

void dma_init(void);
int32_t uartWrite(uint8_t *buf, uint32_t len);
		   
#endif




