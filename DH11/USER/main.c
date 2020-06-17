#include "stm32f10x.h"
#include "delay.h"
#include "usart1.h"
#include <stdio.h>
#include "DHT11.h"

int fputc(int ch, FILE *f)
{

	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);

	return ch;
}

int main()
{
	usart1_init();
	delay_init();
	
	while(1)
	{
		dht11_get_data();
		delay_ms(2000);
	}
	
}

