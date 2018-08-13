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

#define TRUE 1
#define FALSE 0
#define PS4_BUFFER_SIZE 20

char PS4_BUFFER[PS4_BUFFER_SIZE];
int BUFFER_INDEX = 0;
volatile uint8_t data;

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);

void clearBuffer();


int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN | RCC_AHBENR_GPIOAEN | RCC_AHBENR_GPIOBEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
  
	RCC->APB2ENR |= RCC_APB2ENR_TIM16EN | RCC_APB2ENR_TIM17EN;
  TIM16->CR1 = 0;                         // Clear control register

	// Set output-compare CH1 to PWM1 mode and enable CCR1 preload buffer
  TIM16->CCMR1 = (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE);
  TIM16->CCER = TIM_CCER_CC1E;           // Enable capture-compare channel 1
  TIM16->PSC = 7;                         // Run timer on 1Mhz
  TIM16->ARR = 50;                        // PWM at 20kHz
  TIM16->CCR1 = 0;                        // Start PWM at 0% duty cycle
 
	TIM16->BDTR |= TIM_BDTR_MOE;
		
  TIM16->CR1 |= TIM_CR1_CEN;              // Enable timer
	
	//TIM17 
	//*******************

	TIM17->CR1 = 0;                         // Clear control register

	// Set output-compare CH1 to PWM1 mode and enable CCR1 preload buffer
  TIM17->CCMR1 = (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1PE);
  TIM17->CCER = TIM_CCER_CC1E;           // Enable capture-compare channel 1
  TIM17->PSC = 7;                         // Run timer on 1Mhz
  TIM17->ARR = 50;                        // PWM at 20kHz
  TIM17->CCR1 = 0;                        // Start PWM at 0% duty cycle
 
	TIM17->BDTR |= TIM_BDTR_MOE;
		
  TIM17->CR1 |= TIM_CR1_CEN;              // Enable timer
	
	//*******************
	
	//Set PB8 to alternate function mode for PWM mode
	GPIOB->MODER = GPIO_MODER_MODER8_1 | GPIO_MODER_MODER9_1 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER6_0;
	GPIOB->AFR[1] |= 0x2;
	//PB9 alternate AFR function setting
	GPIOB->AFR[1] |= 0x20;
	
	//Enble all the lights to output mode
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0 | GPIO_MODER_MODER4_1 | GPIO_MODER_MODER5_1;
	//PA10 and PA11 are the directional control pins
	//Setting them to high and low
	GPIOA->MODER |= GPIO_MODER_MODER10_0 | GPIO_MODER_MODER9_0;
	//PA10 is set high
	GPIOA->ODR |= GPIO_ODR_10;
	//PA9 is set low
//	GPIOA->ODR |= ~GPIO_ODR_9;
	
	//Left track movement pins
	//PB7 set high and PB8 is set low
	GPIOB->ODR |= GPIO_ODR_7;
	
	GPIOC->AFR[0] = (1 << 16) | (1 << 20);  // Set PC4 & PC5 to AF1 (USART3 TX & RX)

    /* Configure USART3 to 115200 Baud and set up RXNEIE interrupt ------------*/
    USART3->BRR = HAL_RCC_GetHCLKFreq()/115200; // Set Baud to 115200 (0.06% error)
    USART3->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE; // Enable TX, RX, and RX register not empty interrupt
    USART3->CR1 |= USART_CR1_UE;  // Start USART (locks config bits)
    NVIC_EnableIRQ(USART3_4_IRQn); // Enable shared USART3 interrupt
    NVIC_SetPriority(USART3_4_IRQn, 1); 

//6 - red north, 7 - blue south, 8 - orange left, 9 - green right
//	GPIOC->ODR |= GPIO_ODR_6 | GPIO_ODR_7 | GPIO_ODR_8 | GPIO_ODR_9;
//GPIOC -> ODR |= GPIO_ODR_6;

  while (1)
  {
//		HAL_Delay(200);
//	  GPIOC -> ODR ^= GPIO_ODR_6 | GPIO_ODR_7 | GPIO_ODR_8 | GPIO_ODR_9;

	}

}

void processData()
{
	//This code determines if the R2 trigger on the paddle is pressed
	//Depending on how much it is squeezed the speed of the track on the rover 
	//speeds up
	//If the trigger is not squeezed, the track stops
	if(((PS4_BUFFER[12] & 0x08) >> 3) == 1)
	{
		uint8_t value = PS4_BUFFER[10];

		if(value >= 225)
			TIM16->CCR1 = 200;
		else
			TIM16->CCR1 = 40;
	}
	else
		TIM16->CCR1 = 0;
	
	
	//If the circle button is pressed, the right track reverses its direction
	//Otherwise the right track continues to go forward if the R2 trigger is being pulled
	if(((PS4_BUFFER[11] & 0x40) >> 6) == 1)
	{
		//Put the R2 in reverse
		GPIOC->ODR |= GPIO_ODR_9;
			//PA10 is set high
		GPIOA->ODR &= ~GPIO_ODR_10;
		//PA11 is set low
		GPIOA->ODR |= GPIO_ODR_9;
	}
	else
	{
		//Turn light off and make direction back to forwards
			GPIOC->ODR &= ~GPIO_ODR_9;
			//PA10 is set high
		GPIOA->ODR |= GPIO_ODR_10;
		//PA11 is set low
		GPIOA->ODR &= ~GPIO_ODR_9;
	}

	
	//L2 and square controls
	//**********************
	//This code determines if the L2 trigger on the paddle is squeezed
	//The more it is squeezed, the faster the left track goes
	if(((PS4_BUFFER[12] & 0x04) >> 2) == 1)
	{
		uint8_t value2 = PS4_BUFFER[9];

		if(value2 >= 225)
			TIM17->CCR1 = 200;
		else
			TIM17->CCR1 = 40;
	}
	else
		TIM17->CCR1 = 0;
	
	//This determines if the square button is pressed on the paddle
	//If it is the left track direction is reversed, otherwise it continues forward if the L2 trigger is still pulled
	if(((PS4_BUFFER[11] & 0x10) >> 4) == 1)
	{
		//Put the L2 in reverse
		GPIOC->ODR |= GPIO_ODR_8;
			//PA10 is set high
		GPIOB->ODR &= ~GPIO_ODR_7;
		//PA11 is set low
		GPIOB->ODR |= GPIO_ODR_6;
	}
	else
	{
		//Turn light off and make direction back to forwards
			GPIOC->ODR &= ~GPIO_ODR_8;
			//PA10 is set high
		GPIOB->ODR |= GPIO_ODR_7;
		//PA11 is set low
		GPIOB->ODR &= ~GPIO_ODR_6;
	}
}

//Clears the buffer that holds the data coming from the tx line of the bluetooth chip
void clearBuffer()
{
	int i;
	
	for(i = 0; i < PS4_BUFFER_SIZE; i++)
	{
		PS4_BUFFER[i] = '\0';
	}
	BUFFER_INDEX = 0;
}

//This is the USART handler that reads in the data and stores it in a buffer
//The end of the buffer is terminated by a '\n'
//When the end of the buffer is reached, the data is processed and actions are executed
void USART3_4_IRQHandler(void) 
{	  
	if(USART3->ISR & USART_ISR_RXNE) 
	{ 
		data = USART3->RDR;

		PS4_BUFFER[BUFFER_INDEX] = data;
						
		if(data != '\n')
			BUFFER_INDEX++;
		else
		{
			processData();
			clearBuffer();
			BUFFER_INDEX = 0;
		}
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
