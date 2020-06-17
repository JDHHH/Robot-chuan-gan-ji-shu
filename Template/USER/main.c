#include "stm32f10x.h"
#include "usart.h"
//#include "led.h"
//#include "delay.h"
//#include "key.h"
//
//void delay()
//{
//	int i = 30000000;
//	while(i--)
//	{
//	}
//}

//int main()
//{
//	int key;
//	led_init();
//	KEY_init();
//	while(1)
//	{
////		GPIO_SetBits(GPIOA,GPIO_Pin_7);
////		delay();
////		GPIO_ResetBits(GPIOA,GPIO_Pin_7);
////		delay();
//			key = KEY_scan(1);
//			if(key != 0)
//			{
//				GPIO_SetBits(GPIOA,GPIO_Pin_7);
//			}
//			else
//			{
//				GPIO_ResetBits(GPIOA,GPIO_Pin_7);
//			}
//	}
//	
//	return 0;
//}

int main()
{
	//int i = 0;
	usart1_init();
	usart2_init();
	while(1)
	{
//		i = 30000;
//		USART_SendData(USART1,'a');
//		while(i--);
		
	}
}







































