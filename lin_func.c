#include "lin.h"

void CLK_Init_lin(void){
	// ErrorStatus HSEStartUpStatus;
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_OFF);	        //���ⲿ����ʱ�� 
	
	FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
	FLASH_SetLatency(FLASH_Latency_2);
	RCC_HCLKConfig(RCC_SYSCLK_Div1); 
	RCC_PCLK2Config(RCC_HCLK_Div1); 
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_9);
	RCC_PLLCmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
	while(RCC_GetSYSCLKSource() != 0x08);
}

void delay_us(u8 time){
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD��ֵ	    	 
	if(time > 10){
		time = 10;
	}
	ticks=time*DELAY_US_TICKS; 		//��Ҫ�Ľ�����	  		 
	tcnt=0;
	//delay_osschedlock();						//��ֹOS���ȣ���ֹ���us��ʱ
	told=SysTick->VAL;        					//�ս���ʱ�ļ�����ֵ
	while(1){
		tnow=SysTick->VAL;	
		if(tnow!=told){	    
			if(tnow<told)tcnt+=told-tnow;		//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;				//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	}
	//delay_osschedunlock();						//�ָ�OS����			
}

void initDelay_ms(u16 time){
	/*
	for(u16 i=0; i<(time*100); i++){
		delay_us(10);
	}
	//*/
	///*
	//���������߳���, ��ʼ��ʹ�õ���ʱ
	u32 ticks;
	u32 told,tnow,tcnt=0;
	u32 reload=SysTick->LOAD;					//LOAD��ֵ	    	 
	ticks=DELAY_MS_TICKS; 		//��Ҫ�Ľ�����	  		 
	//delay_osschedlock();						//��ֹOS���ȣ���ֹ���us��ʱ
	for(u16 i=0; i<time; i++){
		tcnt=0;
		told=SysTick->VAL;        					//�ս���ʱ�ļ�����ֵ
		while(1){
			tnow=SysTick->VAL;	
			if(tnow!=told){	    
				if(tnow<told)tcnt+=told-tnow;		//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
				else tcnt+=reload-tnow+told;	    
				told=tnow;
				if(tcnt>=ticks)break;				//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
			}  
		}
	}
	//delay_osschedunlock();						//�ָ�OS����	
	//*/
}

