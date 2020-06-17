#include "stm32f10x.h"
#include "delay.h"
#include <stdio.h>

void dht11_output_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//��ʼ��GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}//������dht11���������ó����ģʽ

void dht11_input_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//��ʼ��GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}//������dht11���������ó�����ģʽ

void dht11_start()
{
	dht11_output_init();
	GPIO_ResetBits(GPIOA,GPIO_Pin_7);
	delay_ms(19);	//���õ͵�ƽ���������ݵ��ź�
	GPIO_SetBits(GPIOA,GPIO_Pin_7);
	delay_us(25);	//����������ȴ�dht11����Ӧ
}//��������

int dht11_read_8bits()
{
	unsigned char data = 0;
	int i,time;
	for(i = 0; i < 8; i++)
	{
		data <<= 1;
		time = 0;
		while(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)){}	//����50us�ĵ͵�ƽ
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
}	//��ȡ���ݣ�һ�ζ�8��bit

void dht11_get_data()
{
	unsigned char rh,rl,th,tl,check;
	dht11_start();	
	dht11_input_init();
	
	if(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7))
	{
		while(0 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)){}	//�����ĵ͵�ƽ80us����Ӧ
		while(1 == GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)){}	//�ߵ�ƽ
		rh = dht11_read_8bits();		//ʪ����������
		rl = dht11_read_8bits();		//ʪ��С������
		th = dht11_read_8bits();		//�¶ȵ���������
		tl = dht11_read_8bits();		//�¶ȵ�С������
		check = dht11_read_8bits();	//У��
			
		dht11_output_init();
		GPIO_SetBits(GPIOA,GPIO_Pin_7);	//���������óɸߵ�ƽ��ʹģ�鴦�ڿ���״̬��������һ�����ݲɼ�
		
		if(check == rh + rl + th + tl)
		{
			printf("check successed ..... right data\n");
			printf("�¶ȣ�%d.%d  ʪ�ȣ�%d.%d\n",rh,rl,th,tl);
		}
		else
		{
			printf("check failed.......\n");
		}
	}
}

