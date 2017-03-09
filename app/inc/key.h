#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x_gpio.h"

#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����2
#define KEY3  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)//��ȡ����3

#define KEY1_PRES 0
#define KEY2_PRES 0
#define key3_PRES 0

void key_init(void);
int key_scan(int mode);

#endif