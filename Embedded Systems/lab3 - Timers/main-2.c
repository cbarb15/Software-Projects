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
int ledPos = 0;                                   // current led position from 0..7
int ledDir = 1;
/* USER CODE END 0 */

//	  if ((TIM2->SR & 0x0001) != 0) {                 // check interrupt source

//    if (ledPos == 0)                              // lower limit reached ?
//	  ledDir = 1;                                 // move up

//    if (ledPos == 7)                              // upper limit reached ?
//	  ledDir = -1;                                // move down

//    GPIOB->ODR &= ~(1 << (ledPos+8));             // switch off old LED position
//    ledPos += ledDir;
//    GPIOB->ODR |=  (1 << (ledPos+8));             // switch on  new LED position

//    TIM2->SR &= ~(1<<0);                          // clear UIF flag
// }


int main(void)
{
	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

//  /* Initialize all configured peripherals */
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
			//TIM2 ENABLED	
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN | RCC_APB1ENR_TIM3EN;
	
	//START OF PART 1
	//*******************
	//Setting prescaler and arr values

//	TIM2->PSC = 7999;
//	TIM2->ARR = 250;
//	
//	//ENABLES THE UEV INTERRUPT
//	TIM2->DIER = TIM_DIER_UIE;
//	
//	NVIC_EnableIRQ(TIM2_IRQn);  
//	NVIC_SetPriority(TIM2_IRQn,(1));
//	
//	
//	//Counter enabled
//	TIM2->CR1 |= TIM_CR1_CEN | TIM_CR1_ARPE;

//	
//	GPIOC->MODER = GPIO_MODER_MODER8_0 | GPIO_MODER_MODER6_0;

//	GPIOC->ODR = GPIO_ODR_6;
	
	//END OF PART 1
	//*********************
	
	
//	Part 2 & 3
//	//Enable pin PC6 to alternate function mode
//	GPIOC->MODER = GPIO_MODER_MODER6_1;
//	//Connecting the pin in alternate function
//	//AFR[0] sets pin in AFL0 and AFR[1] set the pins in the AFRH
//	GPIOC->AFR[0] &=  ~(GPIO_AFRL_AFRL6);
// 
//	//enables auto preload register
//	TIM3->CR1 |= TIM_CR1_ARPE;
//	TIM3->ARR = 100;
//	TIM3->PSC = 99;
//	//enables the pwm 1 mode
//	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
//	//enables preload register
//	TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
//	//As the preload registers are transferred to the shadow registers only when an update event occurs, before starting the counter, you have to initialize all the registers by setting the UG bit in the TIMx_EGR register.
//	TIM3->EGR |= 0x1;
//	//Enable the capture compare register
//	TIM3->CCER = TIM_CCER_CC1E;
//	//Sets bit to 25% of ARR
//	TIM3->CCR1 = 20;	
	
	
	


	
//	
//	//Sets pins to alternate function mode
GPIOC->MODER |= GPIO_MODER_MODER6_1;
GPIOC->MODER |= GPIO_MODER_MODER8_1;


TIM3->PSC = 100;
TIM3->ARR = 100;

//Set TIM3_CH1 parameters
TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;	//PWM MODE 1
TIM3->CCMR1 |= TIM_CCMR1_OC1PE;
TIM3->CCER |= TIM_CCER_CC1E;
TIM3->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;

//Set TIM3_CH3 parameters
//TIM3->CCMR2 |= TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3M_2;	//PWM MODE 1
//TIM3->CCMR2 |= TIM_CCMR2_OC3PE;
//TIM3->CCER |= TIM_CCER_CC3E;



//This is duty cycle set and it is a percentage of arr
//This is how much of a percentage of arr and then flips the from low to high or vice versa
TIM3->CCR1 = 50;
//TIM3->CCR3 = 100;
	
  while (1)
  {
		__WFI();
  }
  /* USER CODE END 3 */

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
