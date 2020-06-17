#include "led.h"
#include "stm32f10x.h"

void led_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //??PB,PE????
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;			    //LED0-->PB.5 ????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //????
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;	 //IO????50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     //???GPIOB.5
}


