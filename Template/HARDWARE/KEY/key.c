#include "stm32f10x.h"
#include "key.h"

void KEY_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //??PB,PE????
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;			    //LED0-->PB.5 ????
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 	 //????
  GPIO_Init(GPIOA, &GPIO_InitStructure);			     //???GPIOB.5
}

int KEY_scan(int mode)
{
	static	int key_up = 1;
	if(mode)
	{
		key_up = 1;	//֧������
	}
	if(key_up && (GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0))
	{//��������
		int i = 0;
		for(;i < 300000; i++)
		{
		}//ȥ����
		key_up = 0;
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) == 0)
		{
			return 1;
		}
	}
	else
	{
		key_up = 1;
	}
	
	return 0;//�ް�������
}


	