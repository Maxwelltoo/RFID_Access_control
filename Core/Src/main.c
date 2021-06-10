/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
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
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OLED.h"
#include "Servo.h"
#include "Buzzer.h"
#include "MFRC522.h"
#include "stdio.h"  // 重定向printf

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
// 重定向，串口打印
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&ch, 1, HAL_MAX_DELAY);
    return ch;
}

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void RC522_Init()
{
	PcdReset();      // 复位RC522
	PcdAntennaOff(); // 关闭天线
	PcdAntennaOn();  // 打开天线
	M500PcdConfigISOType( 'A' ); // 设置工作方式：ISO14443_A
	printf("\r\n Initialization succeeded! \r\n");
}
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
	//uint16_t compare_value;
	char status;
	unsigned char TagType[2], SelectedSnr[4];
	//unsigned char snr, buf[16], TagType[2], SelectedSnr[4]; // 扇区号，扇区数据，卡类型，卡序列
	//unsigned char DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // 扇区密码

	const unsigned char Identify[4][4] = {{0x91, 0x72, 0xf9, 0x7c},// 可识别UID: WJX，FJR，YJY，OHS
										  {0x80, 0x77, 0x41, 0xb6},
										  {0xC0, 0x2A, 0x03, 0xB6},
										  {0xE0, 0x1C, 0x02, 0xB6}};
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
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  RC522_Init();
  OLED_init();
  HAL_TIM_Base_Start(&htim3);
  HAL_TIM_Base_Start(&htim4);
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
  Beep_Start();
  OLED_Start();
  HAL_Delay(3200);
  OLED_Wait();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	//compare_value=__HAL_TIM_GET_COMPARE(&htim4, TIM_CHANNEL_1);
	status= PcdRequest(REQ_ALL,TagType);   // 唤醒卡片
	if(!status){
		status = PcdAnticoll(SelectedSnr); // 防冲
		printf("\r\n Request succeeded!\r\n");
		unsigned char i,j;
		unsigned char key;
		for(i=0;i<4;i++){         // 历数每一个已知UID
			key = 0;              // 将校验标志位置零
			for(j=0;j<4;j++){     // 历数UID每一个块
				if(SelectedSnr[j]!=Identify[i][j]){
					break;        // 不匹配，对照下一个UID
				}
				if(j==3){         // 完全匹配
					key = 1;
					OLED_Welcome();
					Beep_Welcome();
					OpenDoor();
					OLED_Wait();
					//printf("\r\n Welcome!\r\n");
				}
			}
			if(key==1){           // 认证成功
				break;
			}
			else if(i==3){        // 未知UID
				OLED_Error();
				Beep_Error();
				HAL_Delay(3200);
				OLED_Wait();
				//printf("\r\n Unknown identity!\r\n");
			}
		}
		/*if(!status){
			status=PcdSelect(SelectedSnr); // 选择卡片
			printf("UID = %x %x %x %x\r\n",SelectedSnr[0],SelectedSnr[1],SelectedSnr[2],SelectedSnr[3]);
			if(!status){
				snr = 1;                   // 选择扇区1
				status = PcdAuthState(KEYA, (snr*4+3), DefaultKey, SelectedSnr); // 校验扇区密码：密码位于块3
				if(!status){
					status = PcdRead((snr*4+0), buf);  // 读卡，读1扇区0块数据到buf[0]-buf[16]
					//status = PcdWrite((snr*4+0), buf);  // 写卡，将buf[0]-buf[16]写入1扇区0
					if(!status){
						//LED_ON; // 读写成功，点亮LED
						WaitCardOff();
					}
				}
			}
		}*/
	}
	//LED_OFF;
  }
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
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
