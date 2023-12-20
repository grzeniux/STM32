/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "lcd_i2c.h"
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

 struct lcd_disp disp;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
  	if(htim->Instance == TIM6)
  	{
  	//	sprintf((char *)disp.f_line, "Shoty");
 	//	sprintf((char *)disp.s_line, "Driny");
  		lcd_display(&disp);


 		//HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);
  	}
  }






/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*
volatile int flow_frequency;
float vol = 0.0, l_minute;
float totalVolume = 0.0; // Nowa zmienna do śledzenia łącznej objętości
float xy = 0.0;
GPIO_TypeDef* flowsensor_port = GPIOB; // Zmień na odpowiedni port GPIO
uint16_t flowsensor_pin = GPIO_PIN_4;   // Zmień na odpowiedni pin

unsigned long currentTime;
unsigned long cloopTime;

*/












uint16_t flowsensor = GPIO_PIN_4;   // Zmień na odpowiedni pin

volatile int flow_frequency;
float vol, l_minute;
float xy = 0.0;
//unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;
unsigned long totalImpulses = 0; // Nowa zmienna do przechowywania liczby impulsów
float imp = 0;
float total_ml;
float ML;

void flow () // Interrupt function to increment flow
{
   flow_frequency++;
   totalImpulses++;
   HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

}


struct lcd_disp disp;








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
  MX_USART2_UART_Init();
  MX_I2C1_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  	disp.addr = (0x27 << 1);
	disp.bl = true;
	lcd_init(&disp);




	currentTime = HAL_GetTick();
    cloopTime = currentTime;
	 totalImpulses = 0;





  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {




		  currentTime = HAL_GetTick();
	          // Every second, calculate and print litres/hour
	          if (currentTime >= (cloopTime + 1000))
	          {
	              cloopTime = currentTime; // Updates cloopTime
	              if (flow_frequency != 0)
	              {
	                  l_minute = (flow_frequency / 1800); // (Pulse frequency x 60 min) / 9.8 Q = flowrate in L/hour

	                  // Przeliczenie ilości impulsów na ilość mililitrów
	                //  float ml_per_impulse = 0.95;
	                //  total_ml = totalImpulses * ml_per_impulse;

	                 // l_minute = l_minute / 60;
	              //    l_minute = l_minute * 1000;
	              //    vol = vol + (l_minute / 7.58);
	                  vol = vol + l_minute;
	                  xy = vol * 1000;
	                  xy /= 200;

	                  ML = totalImpulses * 0.48 ;


	                  sprintf((char *)disp.f_line, "I: %lu, %2.2f ml, ", totalImpulses, ML);
	                  lcd_display(&disp);

	                  // Dodanie informacji o ilości impulsów i ilości mililitrów
	                  sprintf((char *)disp.s_line, "p: %2.2f, %2.2f ml", vol, total_ml);
	                  lcd_display(&disp);

	                  flow_frequency = 0; // Reset Counter
	              }
	              else
	              {
	                  // Dodanie informacji o ilości impulsów i ilości mililitrów
	                 // sprintf((char *)disp.f_line, "%.2f, I: %lu, %.2f ml, ", xy, totalImpulses, total_ml);
	                  sprintf((char *)disp.f_line, "I: %lu, %2.2f ml, ",totalImpulses, ML);

	                  sprintf((char *)disp.s_line, "V: %2.2f X", vol);
	                  lcd_display(&disp);
	              }
	          }



    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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

  /** Configure the main internal regulator output voltage
  */
  if (HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE|RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSICalibrationValue = 0;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV7;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Enable MSI Auto calibration
  */
  HAL_RCCEx_EnableMSIPLLMode();
}

/* USER CODE BEGIN 4 */

//
//void EXTI2_IRQHandler(void)
//{
//    HAL_GPIO_EXTI_IRQHandler(flowsensor_pin);
//}
//
//
//
//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//    if (GPIO_Pin == flowsensor_pin)
//    {
//        flow();
//    }
//}
//
//
//
//void flow(void)
//{
//    flow_frequency++;
//    // Add any additional logic needed for the flow function
//}
//
//
//

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == flowsensor)
    {
      //  HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_3); // Toggle an LED or use an oscilloscope to check if the interrupt is working
        flow();
    }
}




/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
