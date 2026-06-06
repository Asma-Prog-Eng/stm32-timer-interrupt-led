/**
 * ******************************************************************************
 * @file           : main_app.c
 * @brief          : LED Toggling using Timer 2 Delay
 * @project        : STM32 Firmware Development Course
 * ------------------------------------------------------------------------------
 * @hardware       : STM32 Microcontroller
 * @clock_config   : System Clock (SYSCLK) = 36 MHz
 * @peripherals    : TIM2 (General Purpose Timer), GPIO (LED Pin)
 * ------------------------------------------------------------------------------
 * @description    : This application configures Timer 2 to generate precise
 *                   delays for toggling an onboard/external LED. The program
 *                   demonstrates peripheral clock configuration, timer prescaler
 *                   and auto-reload register calculations, and basic GPIO control
 *                   to toggle the LED at rates of 100 ms and 10 ms.
 * ******************************************************************************
 */

#include "stm32f1xx_hal.h"
#include "main_app.h"
#include "msp.h"
#include "it.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>


#define SYS_CLOCK_FREQ_36MHZ 36

#define SYS_CLOCK_FREQ_40MHZ 40

#define SYS_CLOCK_FREQ_52MHZ 52

#define SYS_CLOCK_FREQ_60MHZ 60

// RCC OSC struct. declaration
RCC_OscInitTypeDef osc_init;

// CLK Config struct. declaration
RCC_ClkInitTypeDef clk_init;


TIM_HandleTypeDef htimer2;

// uart struct. declaration
UART_HandleTypeDef huart2;



// data buffer
char msg[100];

int main(void){

	char *user_data = "\n\n*************** The application is running ***************\r\n\n";

	// HAL library inits.
	HAL_Init();

	// SYSCLK configuration
	SYSCLK_Config(SYS_CLOCK_FREQ_36MHZ);

	// configure PA10
	HAL_GPIOA_MspInit();

	// uart inits.
	UART2_Init();

	// send user data to the serial terminal
	HAL_UART_Transmit(&huart2, (uint8_t*) user_data, (uint16_t) strlen(user_data), HAL_MAX_DELAY ) ;

	// display clk frequencies
	Print_Freq();

	// Timer 2 inits.

	TIMER2_Init() ;

	// start timer
	if ( HAL_TIM_Base_Start(&htimer2) != HAL_OK) {

		Error_handler();
	};



	while(1);

    return 0 ;

}


void SYSCLK_Config(uint8_t clock_freq) {

	// 1. Enable HSI SYSCLK and configure it as source clock

		memset(&osc_init, 0, sizeof(osc_init));

		osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;

		osc_init.HSIState = RCC_HSI_ON  ;

		osc_init.HSICalibrationValue = 16;

		osc_init.PLL.PLLSource =  RCC_PLLSOURCE_HSI_DIV2 ;


		osc_init.PLL.PLLState = RCC_PLL_ON;


		// 2 . Configure AHB , APB1 AND APB2 Prescalers

		memset(&clk_init, 0, sizeof(clk_init));

		clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK \
							| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2 ;


		clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK ;

		switch(clock_freq) {

			case(SYS_CLOCK_FREQ_36MHZ):

			{
				osc_init.PLL.PLLMUL =  RCC_PLL_MUL9;

				clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1 ;

				clk_init.APB1CLKDivider = RCC_HCLK_DIV4;

				clk_init.APB2CLKDivider = RCC_HCLK_DIV4 ;

				break;
			}



			case(SYS_CLOCK_FREQ_40MHZ):

		   {
				osc_init.PLL.PLLMUL =  RCC_PLL_MUL10;

				clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1 ;

				clk_init.APB1CLKDivider = RCC_HCLK_DIV2 ;

				clk_init.APB2CLKDivider = RCC_HCLK_DIV4 ;

				break;

		   }

			case(SYS_CLOCK_FREQ_52MHZ):

	        {
				osc_init.PLL.PLLMUL =  RCC_PLL_MUL13;

				clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1 ;

				clk_init.APB1CLKDivider = RCC_HCLK_DIV4 ;

				clk_init.APB2CLKDivider = RCC_HCLK_DIV4 ;

				break;
	        }

			case(SYS_CLOCK_FREQ_60MHZ):

	        {

				osc_init.PLL.PLLMUL =  RCC_PLL_MUL15;

				clk_init.AHBCLKDivider = RCC_HCLK_DIV2 ;

				clk_init.APB1CLKDivider = RCC_HCLK_DIV2 ;

				clk_init.APB2CLKDivider = RCC_HCLK_DIV2 ;

				break;

	        }

			default:
				return;

		}


		if (HAL_RCC_OscConfig(&osc_init) != HAL_OK) {

			 // there is a problem
					 Error_handler();

		}

		HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_1);



		// Sysclk configuration

		HAL_SYSTICK_Config( HAL_RCC_GetSysClockFreq()/1000);

		HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

}

// Timer 2 parameter inits.


void TIMER2_Init(void) {

	htimer2.Instance = TIM2;
	htimer2.Init.Prescaler = 47;
	htimer2.Init.Period = 37500 -1; // 3750 -1 for 10 ms mode
	if (HAL_TIM_Base_Init(&htimer2) != HAL_OK )

	{

		Error_handler();

	}


}
void Print_Freq(void) {

	    // display SYSCLK Feq.
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "SYSCLK : %lu \r\n", HAL_RCC_GetSysClockFreq());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg,  strlen(msg), HAL_MAX_DELAY);

		 // display HCLK Feq.
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "HCLK   : %lu \r\n",  HAL_RCC_GetHCLKFreq());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg,  strlen(msg), HAL_MAX_DELAY);

		 // display PCLK1 Feq.
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "PCLK1  : %lu \r\n", HAL_RCC_GetPCLK1Freq());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg,  strlen(msg), HAL_MAX_DELAY);

		 // display PCLK2 Feq.
		memset(msg, 0, sizeof(msg));
		sprintf(msg, "PCLK2  : %lu \r\n", HAL_RCC_GetPCLK2Freq());
		HAL_UART_Transmit(&huart2, (uint8_t*) msg,  strlen(msg), HAL_MAX_DELAY);

}

void UART2_Init(void)
{

	 huart2.Instance =  USART2;
	 huart2.Init.BaudRate = 115200;
	 huart2.Init.WordLength = UART_WORDLENGTH_8B ;
	 huart2.Init.StopBits = UART_STOPBITS_1;
	 huart2.Init.Parity = UART_PARITY_NONE ;
	 huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	 huart2.Init.Mode = UART_MODE_TX_RX  ;
	 HAL_UART_Init(&huart2);
	 if( HAL_UART_Init(&huart2)!= HAL_OK){

		 // there is a problem
		 Error_handler();

	 }
	 HAL_UART_MspInit(&huart2);
}

void Error_handler(void){

	// Infinite loop if error occurs, blinking a led can be used too here instead

	while(1);

}


