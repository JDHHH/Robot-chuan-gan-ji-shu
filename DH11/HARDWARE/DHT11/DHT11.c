#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>

void dht11_output_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//初始化GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}//把连接dht11的引脚设置成输出模式

void dht11_input_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//初始化GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}//把连接dht11的引脚设置成输入模式

void dht11_start()
{
	dht11_output_init();
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	delay_ms(19);	//设置低电平，请求数据的信号
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	delay_us(25);	//请求结束，等待dht11的相应
}//请求数据

int dht11_read_8bits()
{
	unsigned char data = 0;
	int i,time;
	for(i = 0; i < 8; i++)
	{
		data <<= 1;
		time = 0;
		while(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)){}	//跳过50us的低电平
		while(1 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))
		{
			delay_us(1);
			time++;
		}
		
		if(time > 20)
		{
			data |= 1;
		}
	}
	return data;
}	//读取数据，一次读8个bit

void dht11_get_data()
{
	unsigned char rh,rl,th,tl,check;
	dht11_start();	
	dht11_input_init();
	
	if(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))
	{
		while(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)){}	//读到的低电平80us的响应
		while(1 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)){}	//高电平
		rh = dht11_read_8bits();		//湿度整数部分
		rl = dht11_read_8bits();		//湿度小数部分
		th = dht11_read_8bits();		//温度的整数部分
		tl = dht11_read_8bits();		//温度的小数部分
		check = dht11_read_8bits();	//校验
			
		dht11_output_init();
		GPIO_SetBits(GPIOA,GPIO_Pin_7);	//把引脚设置成高电平，使模块处于空闲状态，方便下一次数据采集
		
		if(check == rh + rl + th + tl)
		{
			printf("check successed ..... right data\n");
			printf("温度：%d.%d  湿度：%d.%d\n",rh,rl,th,tl);
		}
		else
		{
			printf("check failed.......\n");
		}
	}
}

