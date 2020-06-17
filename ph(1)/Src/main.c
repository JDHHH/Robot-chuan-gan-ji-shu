/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "crc.h"
#include "usart.h"
#include "gpio.h"
#include        <math.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#include <stdio.h> 
unsigned int chin[4];
float chin1[4],curr,percent,span,zero;
unsigned int tagcj;
unsigned char serialfg,a[4];
unsigned char tagcj2,dizhi;
unsigned long eepromadd=0x08080000,romadd;//0x0808 07FF--2047 
unsigned char aRxBuffer;

void Delay(uint32_t nCount)
{
	int m, n;
	
	for(m=nCount; m>0; m--)
	{
		for(n=65535; n>0; n--)
			;
	}
}
unsigned char Read8()
{
	unsigned char cLoop,j;
	unsigned char cData;
	for(cLoop=0;cLoop<8;cLoop++)
	{
	  for(j=0;j<13;j++)
	  {
				/*CS5524-SCK*/
			  HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);//SPI2_SCK=0;
  	}
	  for(j=0;j<13;j++)
	  {
				/*CS5524-SCK*/
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_SET);//SPI2_SCK=1;
	  }
	  //bData=SPI2_MISO;
	  cData<<=1;
		if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) != 0)
		{
			cData|=0x01;//return SHIELD_DETECTED;CS5524-SO
		}
		
		
	 
	}
	return cData;
}
void Write8(unsigned char cData)
{
	unsigned char cLoop,j;

	for(cLoop=0;cLoop<8;cLoop++)
	{
 	  if((cData&0x80)==0)
		//if(((cData << cLoop ) & 0x80) == 0x80)
    {
        /*CS5524-SI*/
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7, GPIO_PIN_RESET);//SPI2_MOSI=0;
    }
    else
    {
        /*CS5524-SI*/
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7, GPIO_PIN_SET);//SPI2_MOSI=1;
    }
 	 	for(j=0;j<13;j++)
	  {
				/*CS5524-SCK*/
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_RESET);//SPI2_SCK=0;
  	}
	  for(j=0;j<13;j++)
	  {
				/*CS5524-SCK*/
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_SET);//SPI2_SCK=1;
	  }
   cData<<=1;
  }
}
/*--------------*/
unsigned char a1,a2,a3;
void initCS(void)
{
  unsigned int i;
  for(i=0;i<2047;i++) //延时2048个CS5523_SCLK周期，等待晶振稳定
	{
		/*CS5524-SCK*/
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);//SPI2_SCK=1;
		/*CS5524-SCK*/
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_RESET);//SPI2_SCK=0;
		//printf("jdkljflkdfj:%d\n",i);
	}
	/*CS5524-SI*/
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7, GPIO_PIN_SET);//SPI2_MOSI=1;
	for(i=0;i<254;i++)
	{
		/*CS5524-SCK*/
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_SET);//SPI2_SCK=1;
	  /*CS5524-SCK*/
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_RESET);
		
	}
	/*CS5524-SI*/
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7, GPIO_PIN_RESET);//SPI2_MOSI=0;
	/*CS5524-SCK*/
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_SET);//SPI2_SCK=1;
	/*CS5524-SCK*/
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1, GPIO_PIN_RESET);//SPI2_SCK=0;
	
	Write8(0xff);
	Write8(0xfe);
	
	Write8(0x03);
	Write8(0x07); 
	Write8(0x30); 
	Write8(0x40);

  HAL_Delay(2000);

loop:
	Write8(0x0b);
	HAL_Delay(2000);
	a1=Read8();
	a2=Read8();
	a3=Read8();
	printf("1@@@@@@@ccc:%x  %x  %x\n",a1,a2,a3);
  
if ((a3 & 0x40)==0x00) goto loop;//测RV=1，如不为1，则要再读配置寄存器           

  //配置寄存器
#if 1
	Write8(0x03); 
  Write8(0x07); 
	Write8(0x30); 
	Write8(0x40); 
#else
  Write8(0x03); 
  Write8(0x00); 
	Write8(0x30); 
	Write8(0x00); 
#endif
	HAL_Delay(2000);
	
	
	//读取配置寄存器
	Write8(0x0b);
	HAL_Delay(2000);
	a1=Read8();
	a2=Read8();
	a3=Read8();
	printf("config:%x  %x  %x\n",a1,a2,a3);
	
	//通道设置寄存器 写  101
	Write8(0x05);
//	Write8(0x01);
//	Write8(0xb0);// 
//	Write8(0x9b);//
//	Write8(0x11);//0000 0010
//  Write8(0xb1);//0000 0011
//  Write8(0x9a);//1000 0001
	
	Write8(0x00);
	Write8(0xa0);// 10   60
	Write8(0x83);// 81 83
	Write8(0x10);//
	Write8(0x11);//11(100ml)  91 5v  71（1v）
	Write8(0x81);//
	// 0000 0000  0001          0000  1000  0001         0001 0000    0001         0001  1000 0001
	// 通道1 15hz   100mv  单   通道2  15hz  100mv  单    通道3  15hz  100mv  单   通道4  15hz    100mv  单
	//               1v    双  0110
	
	//偏移校准
	Write8(0x81);//1000 0001
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	
	Write8(0x89);//1000 1001
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	
	Write8(0x91);//1001 0001
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	
	Write8(0x99);// 1001 1001
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	Write8(0xA1);//1010 0001
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	Write8(0xA9);//1010  1001
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	Write8(0xB1);//1011 0001
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	Write8(0xB9); //1011 1001
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);

	//增益校准
	Write8(0x82);//1000 0010
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	Write8(0x8A);//1000 1010 
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	Write8(0x92);//1001 0010
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	Write8(0x9A);//1001 1010
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	Write8(0xA2);//1010 0010
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	Write8(0xAA);//1010 1010
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	Write8(0xB2);//1011  0010
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	Write8(0xBA);//1011  1010
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	
	printf("initCS  finish\n");

}
float temp;
//电压换算成温度
float temp_data(float adc)
{
   float Rt=0;
   float Rp=10000;
   float T2=273.15+25;
   float Bx=3950;
   float Ka=273.15;
   float vol=0;
   vol=(float)(adc*(5/65535));
   Rt=(5-vol)*10000/vol;
   temp=1/(1/T2+log(Rt/Rp)/Bx)-Ka+0.5;
	return temp;
}
#define                K_TEMP                (273.15)
#define                NTC_R1                (10)                //25°C时的电阻值(10K)
#define                NTC_T1                (25+K_TEMP)        //温度值
#define                NTC_B                        (3900)      //B值
#define                Rm                                (10)                //标准电阻值（单位k)

float Count_NTC(float NTC_R2){

        float i,j,r1,r2;

        r1=log(NTC_R1);
        r2=log(NTC_R2);
        j=(r1-r2)/NTC_B;
        i=1/NTC_T1-j;
        j=1/i-K_TEMP;
        return (j);
}
#if 0
float NTC_GetTemp_Do(float u1){
        float i1,r2;

        if(u1 !=0){
              
                i1=((float)MAX_U_REF-u1)/Rm;                                                //得到电流
                r2=u1/i1;                                        //计算电阻
                u1=Mp1.Cb+Count_NTC(r2)+CBK;;
                if(fabs(u1-LedMenu.C) >0.25) LedMenu.C=u1;
                if(LedMenu.C > MAX_TH){ LedMenu.B=0;  Err.T_r=1; }
                if(LedMenu.C < MIN_TL){ LedMenu.B=0;  Err.T_r=2; }
        }else{

        }
        return LedMenu.C;
}
#endif
void readChnData()
{
		unsigned char ch0;
		unsigned char cl0;
	
		unsigned int  ttt[4];
	
	unsigned int chin2[4];
		unsigned int chin3[4];
		unsigned int chin4[4];
		unsigned int chin5[4];

		Write8(0xff);
		ch0=Read8();
		cl0=Read8();
		ttt[0] = ch0*256+cl0;
	  cl0=Read8();
		//printf("CCCCCCCCCCCCCCCCCCCC1:%d\n",cl0);
	  //chin2[1]=ttt[0];
		//chin3[1]=(chin3[1]+chin2[1])/2;
		//chin4[1]=(chin4[1]+chin3[1])/2;
		//chin5[1]=(chin5[1]+chin4[1])/2;
//chin[1]=(chin[1]+chin5[1])/2;

		ch0=Read8();
		cl0=Read8();
		ttt[1] = ch0*256+cl0;
		cl0=Read8();
		//printf("CCCCCCCCCCCCCCCCCCCC2:%d\n",cl0);
		//chin2[1]=ttt[1];
		//chin3[1]=(chin3[1]+chin2[1])/2;
		//chin4[1]=(chin4[1]+chin3[1])/2;
		//chin5[1]=(chin5[1]+chin4[1])/2;
	//	chin[1]=(chin[1]+chin5[1])/2;
		
		

		ch0=Read8();
		cl0=Read8();
		ttt[2] = ch0*256+cl0;
		cl0=Read8();
		//printf("CCCCCCCCCCCCCCCCCCCC3:%d  %d\n",cl0,NTC_CalcTemp(ttt[2]));
		//chin2[2]=ttt[2];
	//	chin3[2]=(chin3[2]+chin2[2])/2;
		//chin4[2]=(chin4[2]+chin3[2])/2;
	//	chin5[2]=(chin5[2]+chin4[2])/2;
	//	chin[2]=(chin[2]+chin5[2])/2;

		ch0=Read8();
		cl0=Read8();
		ttt[3] = ch0*256+cl0;
		cl0=Read8();
		//printf("CCCCCCCCCCCCCCCCCCCC4:%d\n",cl0);
	//	chin2[3]=ttt[3];
	//	chin3[3]=(chin1[3]+chin2[3])/2;
	//	chin4[3]=(chin4[3]+chin3[3])/2;
	//	chin5[3]=(chin5[3]+chin4[3])/2;
	//	chin[3]=(chin[3]+chin5[3])/2;
	printf("@@@@@@@@@@:%f\n",temp_data(1000));
		printf("=======1  [%d]  [%d]  [%d]  [%d]\n",ttt[0],ttt[1],ttt[2],ttt[3]);
		//printf("=======2  [%d]  [%d]  [%d]  [%d]\n",chin[0],chin[1],chin[2],chin[3]);
		
		
}

void ScanADC()
{
  /*CS5524-CS*/
	//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0, GPIO_PIN_RESET);//CS=0;
	
#if 0
	Write8(0x80);
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))==0);
	Write8(0x00);
	a1=Read8();
	a2=Read8();
	a3=Read8();
	printf("1@@@@@@@@@@@@@@:%d   %f  [%d,%d,%d]\n",a1*256+a2, temp_data(a1*256+a2),a1,a2,a3);
	
	HAL_Delay(2000);
	Write8(0x88);
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))==0);
	Write8(0x00);
	a1=Read8();
	a2=Read8();
	a3=Read8();
	printf("2@@@@@@@@@@@@@@:%d   %f [%d,%d,%d]\n",a1*256+a2, temp_data(a1*256+a2),a1,a2,a3);

	HAL_Delay(2000);
	Write8(0x90);
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))==0);
	Write8(0x00);
	a1=Read8();
	a2=Read8();
	a3=Read8();
	printf("3@@@@@@@@@@@@@@:%d   %f [%d,%d,%d]\n",a1*256+a2, temp_data(a1*256+a2),a1,a2,a3);
	
	HAL_Delay(2000);
	Write8(0x98);
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))==0);
	Write8(0x00);
	a1=Read8();
	a2=Read8();
	a3=Read8();
	printf("4@@@@@@@@@@@@@@:%d   %f [%d,%d,%d]\n",a1*256+a2, temp_data(a1*256+a2),a1,a2,a3);
	
#else
	//执行转换
	Write8(0x80);
	do
	{
		//SD0变为低电平，意试转换数据已备好
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))==0);
	//读取数据
  readChnData();
#endif
	

	
	//HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0, GPIO_PIN_SET);//CS=1;
}

int fputc(int c, FILE *stream)    //??fputc??
{
 /*
    huart1??????????UART1???,
    ?????????????,?????????????UART????
*/
    HAL_UART_Transmit(&huart1, (unsigned char *)&c, 1, 1000);   
    return 1;
}

void cs5523_init(){
	int flag22 = 0;
	
	while(flag22 == 0){
		Write8(0x0b);
		unsigned char a4,a5,a6;
	  a4=Read8();
	  a5=Read8();
	  a6=Read8();
		printf("QQQQQQQQQQQQQQQQQQQQQQ %x  %x %x\n",a4,a5,a6);
		if((a4 == 0x00)&& (a5 == 0x00) && (a6 == 0xC0)){
			flag22 =  1;
			Write8(0x03);
      Write8(0x00); 
	    Write8(0x00); 
	    Write8(0x00);
			printf("rrrrrrrrrrrrrrrr\n");
			
		}
		else{
			printf("111111111111111111\n");
			flag22 = 0;
			Write8(0xff);
			HAL_Delay(1000);
			Write8(0xfe);
			HAL_Delay(1000);
			Write8(0x03);
      Write8(0x00); 
	    Write8(0x00); 
	    Write8(0x80);
			HAL_Delay(1000);
			Write8(0xff);
			HAL_Delay(1000);
			Write8(0xfe);
			
		}
	}
	Write8(0x03);
      Write8(0x04); 
	    Write8(0x30); 
	    Write8(0x00);
	HAL_Delay(1000);
	
	Write8(0x05);//通道设置寄存器 写  101
	Write8(0x07);
	Write8(0xb0);// 0000 0010  1=100mV,6=+-1000mV,b=2.5v   
	Write8(0x7b);//1000  0001  子偏移校准
	
	Write8(0x0f);//0000 0010
	Write8(0xb0);//0000 0011
	Write8(0xfb);//1000 0001
	
	HAL_Delay(1000);
	
	/* self calibration SETUP 1 */
	Write8(0x81);//1000 0001
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	HAL_Delay(1000);
	/* self calibration SETUP 1 */
	Write8(0x82);//1000 0010
	do
	{
	}while((HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6))!=0);
	HAL_Delay(1000);
	printf("222222222222222222222\n");
}
  
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_CRC_Init();
  /* USER CODE BEGIN 2 */
	HAL_Delay(2000);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4, GPIO_PIN_SET);//CS=1;
	//HAL_Delay(1000);
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0, GPIO_PIN_RESET);//CS=0;
	
  initCS();
	//cs5523_init();
  /*CS5524-CS*/
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
			ScanADC();
			HAL_Delay(1000);
			printf("wait wait  wait ...\r\n");
  }
	
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0, GPIO_PIN_SET);//CS=1;
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
