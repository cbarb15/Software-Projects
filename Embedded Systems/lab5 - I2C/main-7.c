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
#define LOW_X_THRESH -10000
#define HIGH_X_THRESH 10000
#define LOW_Y_THRESH -10000
#define HIGH_Y_THRESH 10000

int main(void)
{
  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();

  /* Initialize all configured peripherals */
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN | RCC_AHBENR_GPIOCEN;
	//Enable the I2C2
  RCC->APB1ENR |= RCC_APB1ENR_I2C2EN;
//	//Set PB11 to alternate function mode, open-drain output type, and select I2C2_SDA as its alternate function
//	//Alternate function mode and set PB13 to alternate function mode, open-drain output type, and select I2C2_SCL as its alternate function
//	//Set PB14 to output mode
	GPIOB->MODER |= GPIO_MODER_MODER11_1 | GPIO_MODER_MODER13_1 | GPIO_MODER_MODER14_0;
//	//Setting ouput type to open drain
	GPIOB->OTYPER |= GPIO_OTYPER_OT_11 | GPIO_OTYPER_OT_13;
//	//Selecting I2C2_SDA as its alternate function and select I2C2_SCL alternate function
	GPIOB->AFR[1] = (1 << 12) | (5 << 20);
//	//Setting PB14 high
	GPIOB->ODR |= GPIO_ODR_14;
	
	//Enable a light to turn on for the NACKF flag
	GPIOC->MODER |= GPIO_MODER_MODER6_0 | GPIO_MODER_MODER7_0 | GPIO_MODER_MODER8_0 | GPIO_MODER_MODER9_0; 
	
	//Set PC0 to output mode, push-pull output type, and initialize/set the pin high.
	GPIOC->MODER |= GPIO_MODER_MODER0_0;
	GPIOC->ODR |= GPIO_ODR_0;
	
	//Set the parameters in the TIMINGR register to use 100 kHz standard-mode I2C
	I2C2->TIMINGR = (1 << 28)/*PRESCL*/ | 0x13/*SCLL*/ | (0xF << 8)/*SCLH*/ | (2 << 16)/*SDADEL*/ | (4 << 20)/*SCLDEL*/;
	
	//Enable the I2C peripheral using the PE bit in the CR1 register.
	I2C2->CR1 |= I2C_CR1_PE;
	
	I2C2->CR2 = (0x6B << 1)/*Sets slave address*/ | 	(1 << 16)/*Set the number of bytes to transmit = 1.*/ | (0 << 10)/*set write bit*/| (1 << 13)/*set start bit*/;
  

	
	while(!(I2C2->ISR & (I2C_ISR_NACKF | I2C_ISR_TXIS)))
	{  
	}
	if(I2C2->ISR & (I2C_ISR_NACKF))
		GPIOC->ODR |= GPIO_ODR_7;

	//Write the address of the “WHO_AM_I” register into the I2C transmit register
	I2C2->TXDR = 0x0F;

	//Wait until the transfer complete (tc) flage is set
	while(!(I2C2->ISR & (I2C_ISR_TC | I2C_ISR_NACKF)));


	//Reload the CR2 register with the same parameters as before, but set the RD_WRN bit to indicate a read operation.
	I2C2->CR2 = (0x6B << 1)/*Sets slave address*/ | 	(1 << 16)/*Set the number of bytes to transmit = 1.*/ | (1 << 10)/*set read bit*/| (1 << 13)/*set restart bit*/;
  
	//Wait until either of the RXNE (Receive Register Not Empty) or NACKF (Slave Not-Acknowledge) flags are set.
 	while(!(I2C2->ISR & (I2C_ISR_NACKF | I2C_ISR_RXNE)));

	
	//Wait until the transfer complete (tc) flage is set
	while(!(I2C2->ISR & (I2C_ISR_TC | I2C_ISR_NACKF)));


	I2C2->CR2 |= I2C_CR2_STOP; 
	

//	if(I2C2->RXDR == 0xD4)
//	{
//		GPIOC->ODR |= GPIO_ODR_9;
//	}
	
	//*************************
	I2C2->CR2 = (0x6B << 1)/*Sets slave address*/ | 	(2 << 16)/*Set the number of bytes to transmit = 1.*/ | (0 << 10)/*set write bit*/| (1 << 13)/*set start bit*/;

	while(!(I2C2->ISR & (I2C_ISR_NACKF | I2C_ISR_TXIS)))
	{
	}
	
	//PART 2 GYROSCOPE
	//****************
	//Send the address over the SDA to tell it where the CTRL_REG1 is in the gyro chip
	I2C2->TXDR = 0x20;
	
			
	//Wait for the address to be sent
	while(!(I2C2->ISR & (I2C_ISR_NACKF | I2C_ISR_TXIS)));
 //Enable the X and Y sensing axis in the CTRL_REG1 register and
	//set the sensor into “normal or sleep mode” using the PD bit in the CTRL_REG1 register.
	I2C2->TXDR = 0xB;
	while(!(I2C2->ISR & (I2C_ISR_TC)))
	{
	}

	I2C2->CR2 |= I2C_CR2_STOP; 
	//****************

	
  while (1)
  {
    uint8_t lower_bit_x,lower_bit_y;
		int16_t total_x, total_y;
		
	I2C2->CR2 = (0x6B << 1)/*Sets slave address*/ | 	(1 << 16)/*Set the number of bytes to transmit = 1.*/ | (0 << 10)/*set write bit*/| (1 << 13)/*set start bit*/;
			//GPIOC->ODR &= 0;

	while(!(I2C2->ISR & (I2C_ISR_NACKF | I2C_ISR_TXIS)));

	I2C2->TXDR = 0xA8;
		
	while(!(I2C2->ISR & (I2C_ISR_TC)))
	{
	}
		
		I2C2->CR2 = (0x6B << 1)/*Sets slave address*/ | 	(2 << 16)/*Set the number of bytes to transmit = 1.*/ | (1 << 10)/*set write bit*/| (1 << 13)/*set start bit*/;

		//Read byte for x
		lower_bit_x = I2C2->RXDR;
		
		while(!(I2C2->ISR & (I2C_ISR_NACKF | I2C_ISR_RXNE)));
		
		total_x = (I2C2->RXDR << 8) | lower_bit_x;
		
		while(!(I2C2->ISR & (I2C_ISR_TC)));
		
	
	I2C2->CR2 = (0x6B << 1)/*Sets slave address*/ | 	(1 << 16)/*Set the number of bytes to transmit = 1.*/ | (0 << 10)/*set write bit*/| (1 << 13)/*set start bit*/;

	while(!(I2C2->ISR & (I2C_ISR_NACKF | I2C_ISR_TXIS)));

	I2C2->TXDR = 0xAA;
		
	while(!(I2C2->ISR & (I2C_ISR_TC)))
	{
	}
	
	
		I2C2->CR2 = (0x6B << 1)/*Sets slave address*/ | 	(2 << 16)/*Set the number of bytes to transmit = 1.*/ | (1 << 10)/*set write bit*/| (1 << 13)/*set start bit*/;

		//Read byte for y
		lower_bit_y = I2C2->RXDR;
		
		while(!(I2C2->ISR & (I2C_ISR_NACKF | I2C_ISR_RXNE)));
		
		total_y = (I2C2->RXDR << 8) | lower_bit_y;
		
		while(!(I2C2->ISR & (I2C_ISR_TC)));
		
		//GPIOC->ODR &= 0;
		HAL_Delay(100);
	
		if(total_x < -10000)
		{
			GPIOC->ODR |= GPIO_ODR_8;
			GPIOC->ODR &= ~(GPIO_ODR_9);
		}
		else if(total_x > 10000)
		{
			GPIOC->ODR |= GPIO_ODR_9;
			GPIOC->ODR &= ~(GPIO_ODR_8);
		}
		if(total_y < -10000)
		{
			GPIOC->ODR |= GPIO_ODR_7;
			GPIOC->ODR &= ~(GPIO_ODR_6);

		}
		else if(total_y > 10000)
		{
			GPIOC->ODR |= GPIO_ODR_6;
			GPIOC->ODR &= ~(GPIO_ODR_7);
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
