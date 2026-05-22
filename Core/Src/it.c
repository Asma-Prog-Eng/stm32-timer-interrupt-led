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

/*
void TIM2_IRQHandler(void) {

	//2. Toggle PA5 for 1 ms : when couner value of timer 2 is reached

	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);



}*/
