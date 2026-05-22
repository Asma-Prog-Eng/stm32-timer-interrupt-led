/*
 * msp.c
 *
 *  Created on: May 3, 2026
 *      Author: asmae
 */


/**
  * @brief  Initialize the MSP.
  * @retval None
  */

#include "stm32f1xx.h"
#include "msp.h"

extern TIM_HandleTypeDef htimer2;

// low level processor specific initialization
void HAL_MspInit(void)
{
 // low level processor specific inits

	// 1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// 2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= 0x7 << 16; //usg fault, memory fault and bus fault system exceptions

	// 3. Configure the priority for the system exceptions
	 HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
	 HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
	 HAL_NVIC_SetPriority(UsageFault_IRQn, 0,0);

}

// low level inits. of timer 2

void HAL_TIM_Base_MspInit (TIM_HandleTypeDef *htim) {

	// 1. Enable the clock for the TIMER 2 peripheral,

	__HAL_RCC_TIM2_CLK_ENABLE();

	// 2. Enable the IRQ and set up the priority (NVIC settings)

	 NVIC_EnableIRQ(TIM2_IRQn);
	 HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);

}



void HAL_GPIOA_MspInit(void) {

	GPIO_InitTypeDef external_ledgpio;

	// Enable the clock for GPIOA

	__HAL_RCC_GPIOA_CLK_ENABLE();

	// 2. Do the pin muxing configuration : configure PA10 as output

	external_ledgpio.Pin = GPIO_PIN_10;

	external_ledgpio.Pull = GPIO_NOPULL;

	external_ledgpio.Mode = GPIO_MODE_OUTPUT_PP ;

	//3. GPIOA inits.

	HAL_GPIO_Init(GPIOA,&external_ledgpio);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {

	 GPIO_InitTypeDef gpio_uart;


	// low level inits. of the USART2 peripheral

	// 1. Enable the clock for the USART2 peripheral

	__HAL_RCC_USART2_CLK_ENABLE();

	// 2. Do the pin muxing configuration
	//AFIO_REMAP_DISABLE(AFIO_MAPR_USART2_REMAP);
	// UART_TX
	gpio_uart.Pin = GPIO_PIN_2 ;
	gpio_uart.Mode = GPIO_MODE_AF_PP ;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	AFIO_REMAP_DISABLE(AFIO_MAPR_USART2_REMAP);
	HAL_GPIO_Init(GPIOA,&gpio_uart);
	// UART_RX
	gpio_uart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA,&gpio_uart);
	// 3. Enable the IRQ and set up the priority (NVIC settings)

	  NVIC_EnableIRQ(USART2_IRQn);
	  HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}

