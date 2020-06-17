#include "stm32f10x.h"
#include "usart1.h"

void usart1_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//初始化GPIO
	USART_InitTypeDef Usart_InitStructure;	//初始化串口
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
				RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1,ENABLE);
	//开启时钟：GPIOA，端口复用功能AFIO，USART1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//	TX  发送
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//	RX  接收
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_StructInit(&Usart_InitStructure);	//调用函数给结构体变量初始化，初始化成默认的状态
	Usart_InitStructure.USART_BaudRate = 115200;	//波特率改为115200
	USART_Init(USART1,&Usart_InitStructure);	//初始化usart1
	
	USART_Cmd(USART1,ENABLE);	//打开串口1，开始工作
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//串口中断配置
	//suart1 ，  当 接收端有数据的时候      开启 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStructure);
}

void USART1_IRQHandler()
{
	unsigned char temp;
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)	//有数据到达
	{
		temp = USART_ReceiveData(USART1);
		USART_SendData(USART1,temp);
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	//清空中断待处理位 
		//清空数据到达的标志
	}
	USART_ClearITPendingBit(USART1,USART_IT_ORE);			//读完数据，清空
	//避免数据被重复读取
}

