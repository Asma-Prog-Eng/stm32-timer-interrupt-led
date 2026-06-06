/*
 * it.c
 *
 *  Created on: May 3, 2026
 *      Author: asmae
 */

#include "stm32f1xx_hal.h"

// override the default handler, = the infinite loop
// When the SysTick interrupt is triggered, control is transferred to this handler,
//and the system gets stuck in this loop. Consequently, other operations,
// like printing messages, cannot proceed beyond a certain point.

void SysTick_Handler(void) {

	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}


void TIM2_IRQHandler(void) {

	//1. Clear status flag
	TIM2->SR &= ~(1U << 0);
	//2. Toggle PA10 for 100 ms 
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_10);

}
