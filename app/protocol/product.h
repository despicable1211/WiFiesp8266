/**
************************************************************
* @file         gizwits_product.h
* @brief        ��Ӧgizwits_product.c��ͷ�ļ�(������Ʒ��Ӳ���汾����)
* @author       Gizwits
* @date         2016-09-05
* @version      V0.1.2
* @copyright    Gizwits
* 
* @note         ������.ֻΪ����Ӳ������
*               Gizwits Smart Cloud  for Smart Products
*               ����|��ֵ?|����|����|��ȫ|����|����|��̬
*               www.gizwits.com
*
***********************************************************/
#ifndef _PRODUCT_H
#define _PRODUCT_H

#include <stdint.h>
#include <stm32f10x.h>

#define TIMER 					TIM3
#define TIMER_IRQ 				TIM3_IRQn
#define TIMER_RCC 				RCC_APB1Periph_TIM3
#define TIMER_IRQ_FUN 			TIM3_IRQHandler



/**@name �豸״̬�����ϱ����ʱ��궨��*/

#define REPORT_TIME_MAX 6000 //6S


void timerInit(void);

#endif