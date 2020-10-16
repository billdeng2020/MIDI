#include "led.h"
#include "nrf_delay.h"
int main(void)
{
	LED_Init();
	while(1)
	{
		LED1_Open();LED2_Close();LED3_Close();nrf_delay_ms(1000);
		LED2_Open();LED1_Close();LED3_Close();nrf_delay_ms(1000);
		LED3_Open();LED1_Close();LED2_Close();nrf_delay_ms(1000);
	}
	
}
