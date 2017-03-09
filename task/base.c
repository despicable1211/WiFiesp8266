#include "base.h"
#include "motor.h"
#include "dma/dma1.h"
#include "usart/usart.h"
#include "protocol/protocol.h"

dataPoint_t currentDataPoint;

u8 task_base(void){
_SS
	static bool running=TRUE;
	static u16 time_out = 0;
        int i = 0;
	Delay_s(1);
	while(1){
		time_out ++;

                protocolHandle((dataPoint_t *)&currentDataPoint);
                Delay_ms(50);
	}
_EE
}

//WiFi
u8 task_wifi(void)
{
  _SS
      while(1)
      {
        //uart1Write(SendBuff1,15);
        Delay_ms(100);
      }
  _EE
}