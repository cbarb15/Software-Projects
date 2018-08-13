/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "stm32f0xx_hal.h"

#define _BV(x) (1 << x)
#define TRUE 1
#define FALSE 0

char BUFFER[20];
int BUFFER_INDEX = 0;
const char ERR_MSG[17] = {'e','r','r','o','r',':',' ','b','a','d',' ','i','n','p','u','t','\0'};
char COLOR_MESSAGE[15] = {'E', 'n', 't','e','r',' ','c','o','l','o','r','.','.','\0'};
char COMMAND_MESSAGE[16] = {'E', 'n', 't','e','r',' ','c','o','m','m','a','n','d','.','.','\0'};
int NEW_DATA = FALSE;
int RED_ON = FALSE;
int BLUE_ON = FALSE;
int ORANGE_ON = FALSE;
int GREEN_ON = FALSE;
char NEWLINE = '\n';
char RETURN = '\r';
int CHECK_COMMAND = FALSE;
int RED_FLAG = FALSE;
int BLUE_FLAG = FALSE;
int GREEN_FLAG = FALSE;
int ORANGE_FLAG = FALSE;
int ON_FLAG = FALSE;
int OFF_FLAG = FALSE;
int TOGGLE_FLAG = FALSE;
char red_string[4] = {'r', 'e', 'd', '\0' };
char blue_string[5] = {'b', 'l', 'u', 'e', '\0'};
char orange_string[7] = {'o', 'r', 'a', 'n', 'g', 'e', '\0'};
char green_string[6] = {'g', 'r', 'e', 'e', 'n', '\0'};
char on_string[3] = {'o', 'n', '\0' };
char off_string[4] = {'o', 'f', 'f', '\0'};
char toggle_string[7] = {'t', 'o', 'g', 'g', 'l', 'e', '\0'};
const char PHRASE[28] = { '\n','h','e','l','l','o',' ','t','h','i','s',' ','i','s',' ','t','h','e',' ','c','o','m','p','u','t','e','r','\0' };
/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void Error_Handler(void);
int COLOR_MATCH = TRUE;
int COMMAND_MATCH = TRUE;
int START_OF_BUFFER = 0;


//int printStr(char*);
//void printChar(char);

void printChar(char character);
void printStr(const char string[]);
int match_color_string(char string[]);
int match_command_string(char string[]);
void clearBuffer();
void printBuffer();
void execute_Command();

volatile _Bool rx_flag; // C-standard defines _Bool as smallest integer datatype (not an official type/keyword though)
volatile uint8_t data;

int main(void) {
    
    HAL_Init(); /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    SystemClock_Config(); /* Configure the system clock */

     /* Enable Peripheral Clocks in RCC ---------------------------------------*/
    RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
    RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
    
		clearBuffer();
		
    /* Configure GPIOC (LED & USART3) Pins ------------------------------------*/
    uint32_t MODE = _BV(18) | _BV(16) | _BV(14) | _BV(12);  // Set PC6 through PC8 to output
             MODE |= _BV(9) | _BV(11);                      // Set PC4 & PC5 to alternate function
    GPIOC->MODER = MODE;  
    GPIOC->AFR[0] = _BV(16) | _BV(20);  // Set PC4 & PC5 to AF1 (USART3 TX & RX)

    /* Configure USART3 to 115200 Baud and set up RXNEIE interrupt ------------*/
    USART3->BRR = HAL_RCC_GetHCLKFreq()/115200; // Set Baud to 115200 (0.06% error)
    USART3->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_RXNEIE; // Enable TX, RX, and RX register not empty interrupt
    USART3->CR1 |= USART_CR1_UE;  // Start USART (locks config bits)
    NVIC_EnableIRQ(USART3_4_IRQn); // Enable shared USART3 interrupt
    NVIC_SetPriority(USART3_4_IRQn, 1); // Set to high priority
		
		printStr(COLOR_MESSAGE);
		
    while (1) 
		{
        //__WFI(); // Wait until interrupt occurs before checking data
    }
}

void clearBuffer()
{
	int i;
	
	for(i = 0; i < sizeof(BUFFER); i++)
	{
		BUFFER[i] = '\0';
	}
	BUFFER_INDEX = 0;
}

void printBuffer()
{
	int i;
	for(i = 0; i < 5; i++)
	{
		printChar(BUFFER[i]);
	}
}

char red[3] = {'r','e','d'};
char on[2] = {'o','n'};
char here[4] = {'h','e','r','e'};
//Post-lab code
void USART3_4_IRQHandler(void) 
{
			if(USART3->ISR & USART_ISR_RXNE) 
			{ 
        data = USART3->RDR;
				if(data != '\r')
				{
					BUFFER[BUFFER_INDEX] = data;
					BUFFER_INDEX++;
				}
        if(data == '\r')
				{
					BUFFER_INDEX = START_OF_BUFFER;
					if(CHECK_COMMAND == FALSE)
					{
						COLOR_MATCH = match_color_string(BUFFER);
						printChar(NEWLINE);
						printChar(RETURN);
						if(COLOR_MATCH == FALSE)
						{
							clearBuffer();
							BUFFER_INDEX = 0;
							printStr(COLOR_MESSAGE);
						}
						else
						{
							clearBuffer();
							BUFFER_INDEX = 0;
							printStr(COMMAND_MESSAGE);
							CHECK_COMMAND = TRUE;
						}
					}
					else
					{
						COMMAND_MATCH = match_command_string(BUFFER);
						printChar(NEWLINE);
						printChar(RETURN);
						if(COMMAND_MATCH == FALSE)
						{
							clearBuffer();
							BUFFER_INDEX = 0;
							printStr(COMMAND_MESSAGE);
						}
						else
						{
							clearBuffer();
							BUFFER_INDEX = 0;
							CHECK_COMMAND = FALSE;
							execute_Command();
							printChar(NEWLINE);
							printChar(RETURN);
							printStr(COLOR_MESSAGE);
						}
					}
				}
			}
}

int match_color_string(char string[])
{
		int i;
		int color_match = TRUE;
		char* color;
		

		if(string[0] == red_string[0])
			color = red_string;
		else if(string[0] == blue_string[0])
			color = blue_string;
		else if(string[0] == orange_string[0])
			color = orange_string;
		else if(string[0] == green_string[0])
			color = green_string;
		
	for(i = 0; string[i] != '\0'; i++)
	{
		if(string[i] != color[i])
		{
			color_match = FALSE;
			printStr(ERR_MSG);
			clearBuffer();
			BUFFER_INDEX = 0;
			break;
		}
	}
	
	if(color_match == TRUE)
	{
		if(color[0] == 'r')
		{
			RED_FLAG = TRUE;
			printStr(red_string);
		}
		else if(color[0] == 'b')
		{
			BLUE_FLAG = TRUE;
			printStr(blue_string);
		}
		else if(color[0] == 'g')
		{
			GREEN_FLAG = TRUE;
			printStr(green_string);
		}
		else if(color[0] == 'o')
		{
			ORANGE_FLAG = TRUE;
			printStr(orange_string);
		}
	 }
	
	return color_match;
}



int match_command_string(char string[])
{
		int i;
		int command_match = TRUE;
		char* command;
	
		//printStr(string);
	
		if(string[1] == on_string[1])
			command = on_string;
		else if(string[1] == off_string[1])
			command = off_string;
		else if(string[0] == toggle_string[0])
			command = toggle_string;

	for(i = 0; string[i] != '\0'; i++)
	{
		if(string[i] != command[i])
		{
			command_match = FALSE;
			printStr(ERR_MSG);
			clearBuffer();
			BUFFER_INDEX = 0;
			break;
		}
	}
	
		if(command[1] == 'n')
		{
			ON_FLAG = TRUE;
			printStr(on_string);
		}
		else if(command[1] == 'f')
		{
			OFF_FLAG = TRUE;
			printStr(off_string);
		}
		else if(command[0] == 't')
		{
			TOGGLE_FLAG = TRUE;
			printStr(toggle_string);
		}
	return command_match;
}

void execute_Command()
{
		if(RED_FLAG && ON_FLAG)
			{
				GPIOC->ODR |= GPIO_ODR_6;
				clearBuffer();
				BUFFER_INDEX = 0;
				RED_ON = TRUE;
				RED_FLAG = FALSE;
				ON_FLAG = FALSE;
			}
			else if(RED_FLAG && OFF_FLAG)
			{
				GPIOC->ODR = 0;	
				clearBuffer();
				BUFFER_INDEX = 0;
				RED_ON = FALSE;
				RED_FLAG = FALSE;
				OFF_FLAG = FALSE;
			}
			else if(BLUE_FLAG && ON_FLAG)
			{
				GPIOC->ODR = GPIO_ODR_7;
				clearBuffer();
				BUFFER_INDEX = 0;
				BLUE_ON = TRUE;
				BLUE_FLAG = FALSE;
				ON_FLAG = FALSE;
			}
			else if(BLUE_FLAG && OFF_FLAG)
			{
					GPIOC->ODR = 0;	
					clearBuffer();
					BUFFER_INDEX = 0;
					BLUE_ON = FALSE;
					BLUE_FLAG = FALSE;
					OFF_FLAG = FALSE;
			}
			else if(ORANGE_FLAG && ON_FLAG)
			{
					GPIOC->ODR = GPIO_ODR_8;	
					clearBuffer();
					BUFFER_INDEX = 0;
					ORANGE_ON = TRUE;
					ORANGE_FLAG = FALSE;
					ON_FLAG = FALSE;
			}
			else if(ORANGE_FLAG && OFF_FLAG)
			{
					GPIOC->ODR = 0;	
					clearBuffer();
					BUFFER_INDEX = 0;
					ORANGE_ON = FALSE;
					ORANGE_FLAG = FALSE;
					OFF_FLAG = FALSE;
			}
			else if(GREEN_FLAG && ON_FLAG)
			{
				GPIOC->ODR = GPIO_ODR_9;
				clearBuffer();
				BUFFER_INDEX = 0;
				GREEN_ON = TRUE;
				GREEN_FLAG = FALSE;
				ON_FLAG = FALSE;
			}
			else if(GREEN_FLAG && OFF_FLAG)
			{
				GPIOC->ODR = 0;
				clearBuffer();
				BUFFER_INDEX = 0;
				GREEN_ON = FALSE;
				GREEN_FLAG = FALSE;
				OFF_FLAG = FALSE;
			}
			else if(RED_FLAG && TOGGLE_FLAG)
			{
				if(RED_ON == FALSE)
				{
					GPIOC->ODR = GPIO_ODR_6;
					clearBuffer();
					BUFFER_INDEX = 0;
					RED_ON = TRUE;
					RED_FLAG = FALSE;
					TOGGLE_FLAG = FALSE;
				}
				else
				{
					GPIOC->ODR = 0;
					clearBuffer();
					BUFFER_INDEX = 0;
					RED_ON = FALSE;
					RED_FLAG = FALSE;
					TOGGLE_FLAG = FALSE;
				}
			}
			else if(BLUE_FLAG && TOGGLE_FLAG)
			{
				if(BLUE_ON == FALSE)
				{
					GPIOC->ODR = GPIO_ODR_7;
					clearBuffer();
					BUFFER_INDEX = 0;
					BLUE_ON = TRUE;
					BLUE_FLAG = FALSE;
					TOGGLE_FLAG = FALSE;
				}
				else
				{
					GPIOC->ODR = 0;
					clearBuffer();
					BUFFER_INDEX = 0;
					BLUE_ON = FALSE;
					BLUE_FLAG = FALSE;
					TOGGLE_FLAG = FALSE;
				}
			}
			else if(ORANGE_FLAG && TOGGLE_FLAG)
			{
				if(ORANGE_ON == FALSE)
				{
					GPIOC->ODR = GPIO_ODR_8;
					clearBuffer();
					BUFFER_INDEX = 0;
					ORANGE_ON = TRUE;
					ORANGE_FLAG = FALSE;
					TOGGLE_FLAG = FALSE;
				}
				else
				{
					GPIOC->ODR = 0;
					clearBuffer();
					BUFFER_INDEX = 0;
					ORANGE_ON = FALSE;
					ORANGE_FLAG = FALSE;
					TOGGLE_FLAG = FALSE;
				}
			}
			else if(GREEN_FLAG && TOGGLE_FLAG)
			{
				if(GREEN_ON == FALSE)
				{
					GPIOC->ODR = GPIO_ODR_9;
					clearBuffer();
					BUFFER_INDEX = 0;
					GREEN_ON = TRUE;
					GREEN_FLAG = FALSE;
					TOGGLE_FLAG = FALSE;
				}
				else
				{
					GPIOC->ODR = 0;
					clearBuffer();
					BUFFER_INDEX = 0;
					GREEN_ON = FALSE;
					GREEN_FLAG = FALSE;
					TOGGLE_FLAG = FALSE;
				}
			}
			else
			{
				clearBuffer();
				BUFFER_INDEX = 0;
  			printStr(ERR_MSG);
				RED_FLAG = FALSE;
				GREEN_FLAG = FALSE;
				BLUE_FLAG = FALSE;
				ORANGE_FLAG = FALSE;
				ON_FLAG = FALSE;
				OFF_FLAG = FALSE;
				TOGGLE_FLAG = FALSE;
				
			}
}
// USART Interrupt Handler 
//Code for last part of in lab checkoff
//void USART3_4_IRQHandler(void) 
//{
//	if(USART3->ISR & USART_ISR_RXNE) 
//			{ 
//        data = USART3->RDR;  
//        printChar(data);
//				if(data != 'z')
//				{
//					BUFFER[BUFFER_INDEX] = data;
//					BUFFER_INDEX++;
//				}
//				else if(data == 'z')
//				{
//					 if(BUFFER[0] == 'r' && BUFFER[1] == '1')
//						{
//								GPIOC->ODR |= GPIO_ODR_6;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								RED_ON = TRUE;
//						}
//						else if(BUFFER[0] == 'r' && BUFFER[1] == '0')
//						{
//								GPIOC->ODR = 0;	
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								RED_ON = FALSE;

//						}
//						else if(BUFFER[0] == 'b' && BUFFER[1] == '1')
//						{
//								GPIOC->ODR = GPIO_ODR_7;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								BLUE_ON = TRUE;

//						}
//						else if(BUFFER[0] == 'b' && BUFFER[1] == '0')
//						{
//								GPIOC->ODR = 0;	
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								BLUE_ON = FALSE;

//						}
//						else if(BUFFER[0] == 'o' && BUFFER[1] == '1')
//						{
//								GPIOC->ODR = GPIO_ODR_8;	
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								ORANGE_ON = TRUE;

//						}
//						else if(BUFFER[0] == 'o' && BUFFER[1] == '0')
//						{
//								GPIOC->ODR = 0;	
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								ORANGE_ON = FALSE;

//						}
//						else if(BUFFER[0] == 'g' && BUFFER[1] == '1')
//						{
//								GPIOC->ODR = GPIO_ODR_9;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								GREEN_ON = TRUE;

//						}
//						else if(BUFFER[0] == 'g' && BUFFER[1] == '0')
//						{
//								GPIOC->ODR = 0;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								GREEN_ON = FALSE;
//						}
//						else if(BUFFER[0] == 'r' && BUFFER[1] == '2')
//						{
//							if(!RED_ON)
//							{
//								GPIOC->ODR = GPIO_ODR_6;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								RED_ON = TRUE;
//							}
//							else
//							{
//								GPIOC->ODR = 0;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								RED_ON = FALSE;
//							}
//						}
//						else if(BUFFER[0] == 'b' && BUFFER[1] == '2')
//						{
//							if(!BLUE_ON)
//							{
//								GPIOC->ODR = GPIO_ODR_7;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								BLUE_ON = TRUE;
//							}
//							else
//							{
//								GPIOC->ODR = 0;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								BLUE_ON = FALSE;
//							}
//						}
//						else if(BUFFER[0] == 'o' && BUFFER[1] == '2')
//						{
//							if(!ORANGE_ON)
//							{
//								GPIOC->ODR = GPIO_ODR_8;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								ORANGE_ON = TRUE;
//							}
//							else
//							{
//								GPIOC->ODR = 0;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								ORANGE_ON = FALSE;
//							}
//						}
//						else if(BUFFER[0] == 'g' && BUFFER[1] == '2')
//						{
//							if(!GREEN_ON)
//							{
//								GPIOC->ODR = GPIO_ODR_9;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								GREEN_ON = TRUE;
//							}
//							else
//							{
//								GPIOC->ODR = 0;
//								clearBuffer();
//								BUFFER_INDEX = 0;
//								printStr(CMD_MESSAGE);
//								GREEN_ON = FALSE;
//							}
//						}
//						else
//						{
//							clearBuffer();
//							BUFFER_INDEX = 0;
//							printStr(ERR_MSG);
//							printStr(CMD_MESSAGE);
//						}
//						
//				}
//			}
//}

//void printChar(char c) 
//{
//    while(!(USART3->ISR & USART_ISR_TXE)); // Loop until the TX register is empty (TXE bit is set)
//    USART3->TDR = c;
//}

void printChar(char character)
{
	while(!(USART3->ISR & USART_ISR_TXE))
	{
		
	}
	USART3->TDR = character;
}

void printStr(const char string[])
{
	int i = 0;
	
	while(!(USART3->ISR & USART_ISR_TXE))
	{
		
	}
	
	while(string[i] != '\0')
	{
		printChar(string[i]);
		i++;
	}
}

void printInt(int num)
{
	while(!(USART3->ISR & USART_ISR_TXE))
	{
		
	}
	USART3->TDR = num;
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