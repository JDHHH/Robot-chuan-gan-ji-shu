#include "stm32f10x.h"
#include "usart1.h"

void usart1_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;	//��ʼ��GPIO
	USART_InitTypeDef Usart_InitStructure;	//��ʼ������
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | 
				RCC_APB2Periph_AFIO | RCC_APB2Periph_USART1,ENABLE);
	//����ʱ�ӣ�GPIOA���˿ڸ��ù���AFIO��USART1
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	//	TX  ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	//	RX  ����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_StructInit(&Usart_InitStructure);	//���ú������ṹ�������ʼ������ʼ����Ĭ�ϵ�״̬
	Usart_InitStructure.USART_BaudRate = 115200;	//�����ʸ�Ϊ115200
	USART_Init(USART1,&Usart_InitStructure);	//��ʼ��usart1
	
	USART_Cmd(USART1,ENABLE);	//�򿪴���1����ʼ����
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	//�����ж�����
	//suart1 ��  �� ���ն������ݵ�ʱ��      ���� 
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
	if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET)	//�����ݵ���
	{
		temp = USART_ReceiveData(USART1);
		USART_SendData(USART1,temp);
		
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);	//����жϴ�����λ 
		//������ݵ���ı�־
	}
	USART_ClearITPendingBit(USART1,USART_IT_ORE);			//�������ݣ����
	//�������ݱ��ظ���ȡ
}

