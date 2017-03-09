#ifndef LIN_CONF_H
#define LIN_CONF_H

#include "stm32f10x.h"

///////���涨���������������궨��//////
//���涨��λ����
#define BIT0		(u8)0x01
#define BIT1		(u8)0x02
#define BIT2		(u8)0x04
#define BIT3		(u8)0x08
#define BIT4		(u8)0x10
#define BIT5		(u8)0x20
#define BIT6		(u8)0x40
#define BIT7		(u8)0x80
#define ALL_BIT	(u8)0xFF

//����boolean ����
typedef enum {FALSE = 0, TRUE = !FALSE} bool;

//���涨���߳���س���
#define MAXTASKS				32	//����߳���
//#define SYS_TICK_TIME_1ms		36000	//ϵͳʱ��1ms
//#define SYS_TICK_TIME_100us		3600	//ϵͳʱ��100us
#define SYS_TICK_TIME_1ms		(SystemCoreClock/1000)	//ϵͳʱ��1ms
#define SYS_TICK_TIME_100us	(SystemCoreClock/10000)	//ϵͳʱ��100us
#define TASK_COUNT				20
#define TASK_BASE_ID			0
#define TASK_NORMAL_ID(ID)		(TASK_BASE_ID+ID)
#define TASK_NO_RUN_ID(ID)		(TASK_COUNT-ID)

//���峬����ʱ��������
#define DELAY_US_TICKS			(SYS_TICK_TIME_100us/100)	//ϵͳʱ��1us
#define DELAY_MS_TICKS			(DELAY_US_TICKS*1000)		//ϵͳʱ��1ms

//����GPIO ���Žṹ��
typedef struct{
	GPIO_TypeDef * GPIOx;
	u16 Pinx;
}GPIO_Channel_TypeDef;

//���涨��ϵͳ����
typedef struct{
	u8 rtc_flag;
	u16 one_second_count;
	u32 system_time;
	u32 boot_time;
}SYSTEM_lin_TypeDef;


/////////////���涨�����нṹ��///////////////
typedef struct{
	//����ϵͳ�ṹ��
	SYSTEM_lin_TypeDef SYS_DATA;
	
}run_lin_TypeDef;

#endif
