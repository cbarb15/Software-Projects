/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2017 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

/* USER CODE BEGIN PFP */
/* Private function prototypes -----------------------------------------------*/

/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
#define THRESH_1 63
#define THRESH_2 126
#define THRESH_3 189
#define THRESH_4 252
#define ARRAY_SIZE sizeof(sine_table) / sizeof(sine_table[0])

int main(void)
{

	const uint8_t sine_table[32] = {127, 151,175,197,216,232,244,251,254,251,244,232,216,197,175,151,127,102,78,56,37,21,9,2,0,2,9,21,37,56,78,102};  
	int i;

  /* MCU Configuration----------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

 //Enable lights,GPIOC, and GPIOA
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN;
	//Enable ADC1 in the RCC register
	RCC->APB2ENR |= RCC_APB2ENR_ADCEN;
	//Enable DAC1 in the RCC register
	RCC->APB1ENR |= RCC_APB1ENR_DACEN;
	//Set lights to output mode
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0;
	
	//DAC part 
	//*********************
	//Configure pin PC0 to analog mode
	//GPIOC->MODER |= 3;
	//Configure pin PA4
	GPIOA->MODER |= (3 << 8);
	//Set the used DAC channel to software trigger mode and enable the DAC
	DAC1->CR |= (7 << 3) | 1;
	
	
  //***********************
	//Configure pin PC0 with no pull-up/pull-down register
  //GPIOC->PUPDR |= 0;
	
	//Configure the ADC to 8-bit resolution, continuous conversion mode, hardware triggers disabled
	//ADC1->CFGR1 |= (2 << 3)/*sets 8-bit res*/ | (1 << 13)/*cont. conversion mode*/;
	
	//Select/enable the input pin’s channel for ADC conversion.
  //ADC1->CHSELR |= 1 << 10;//1 << 10 cause PC0 is ADC_IN10
	
	//Perform a self-calibration and start the ADC.
	/* (1) Ensure that ADEN = 0 */
	/* (2) Clear ADEN */
	/* (3) Launch the calibration by setting ADCAL */
	/* (4) Wait until ADCAL=0 */
//	if ((ADC1->CR & ADC_CR_ADEN) != 0) /* (1) */
//	{
//			ADC1->CR &= (uint32_t)(~ADC_CR_ADEN); /* (2) */
//	}
//		ADC1->CR |= ADC_CR_ADCAL; /* (3) */
//		while ((ADC1->CR & ADC_CR_ADCAL) != 0) /* (4) */
//		{
//			/* For robust implementation, add here time-out management */
//		}
//	
//	ADC1->CR |= 1; //Turn enable bit ADC1
//	
//	//Let it warm up
//	while((ADC1->ISR & ADC_ISR_ADRDY) != ADC_ISR_ADRDY)
//	{
//	}
//	//Start the conversion
//	ADC1->CR |= (1 << 2);
	
  while (1)
  {
 
		if(i < ARRAY_SIZE)
			i++;
		else
			i = 0;
		
		DAC1->DHR8R1 = sine_table[i];
		HAL_Delay(1);

		//Part 1
//		if(ADC1->DR >= THRESH_1)
//			GPIOC->ODR |= GPIO_ODR_6;
//		if(ADC1->DR >= THRESH_2)
//			GPIOC->ODR |= GPIO_ODR_7;
//		if(ADC1->DR >= THRESH_3)
//			GPIOC->ODR |= GPIO_ODR_8;
//		if(ADC1->DR >= THRESH_4)
//			GPIOC->ODR |= GPIO_ODR_9;
//		if(ADC1->DR < THRESH_1)
//			GPIOC->ODR &= ~(GPIO_ODR_6);
//		if(ADC1->DR < THRESH_2)
//			GPIOC->ODR &= ~(GPIO_ODR_7);
//		if(ADC1->DR < THRESH_3)
//			GPIOC->ODR &= ~(GPIO_ODR_8);
//		if(ADC1->DR < THRESH_4)
//			GPIOC->ODR &= ~(GPIO_ODR_9);

		//HAL_Delay(200);
		
		
	}	
}
/** System Clock Configuration
*/
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }

    /**Configure the Systick interrupt time 
    */
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

    /**Configure the Systick 
    */
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

  /* SysTick_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler */
  /* User can add his own implementation to report the HAL error return state */
  while(1) 
  {
  }
  /* USER CODE END Error_Handler */ 
}

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
