# STM32 Non-Blocking LED Toggling via Timer 2 Interrupts

This repository contains an embedded C implementation for an STM32 microcontroller to toggle an LED at precise intervalb (100 ms) using **Timer 2 (TIM2) Update Interrupts**. By shifting from a blocking polling mechanism to an Interrupt Service Routine (ISR), the CPU is left completely free to execute other tasks in the main execution loop.

## Project Specifications

*   **Microcontroller:** STM32 Series
*   **Language:** C
*   **System Clock (SYSCLK):** 36 MHz
*   **Timer Peripheral:** TIM2 (General Purpose 16-bit Timer)
*   **Interrupt Controller:** NVIC (Nested Vectored Interrupt Controller)
*   **Target Intervals:** 100 ms (5 Hz)

---

## Technical Implementation

### Clock Tree & Timer Configuration
The microcontroller utilizes the Internal High-Speed oscillator (**HSI**) multiplied via the **PLL** to achieve a **36 MHz System Clock**. With the APB divider set to 4, the Timer 2 internal clock operates at **18 MHz** (due to the automatic x2 hardware multiplier applied to timer peripherals when the APB divider is greater than 1).

The timer calculations follow the standard formula:

$$Update\_Event = \frac{TIM\_CLK}{(PSC + 1) \times (ARR + 1)}$$

### Register Configurations (18 MHz Input Clock)

To trigger the hardware interrupt at the exact intervals, the 16-bit **TIM2** registers are configured as follows:

*   **For 100 ms Interrupt Interval (5 Hz Toggle Rate):**
    *   **Prescaler (PSC):** `47` (Clock divided by $47 + 1 = 48$, resulting in a $375\text{ kHz}$ counter frequency)
    *   **Auto-Reload Register (ARR):** `37500` ($37,500 \text{ ticks} \div 375,000 \text{ Hz} = 0.1 \text{ s}$)

*   **For 10 ms Interrupt Interval (50 Hz Toggle Rate):**
    *   **Prescaler (PSC):** `47` (Maintains the same $375\text{ kHz}$ counter frequency)
    *   **Auto-Reload Register (ARR):** `3750` ($3,750 \text{ ticks} \div 375,000 \text{ Hz} = 0.01 \text{ s}$)

### Interrupt Architecture
1.  **DIER Register:** The Timer 2 Update Interrupt Enable bit (`UIE`) is set to trigger an interrupt whenever the counter overflows and matches the `ARR` value.
2.  **NVIC Configuration:** The `TIM2_IRQn` channel is enabled in the Nested Vectored Interrupt Controller with an appropriate priority.
3.  **ISR Handling:** The LED toggle logic is implemented inside the `TIM2_IRQHandler()` function, where the update interrupt flag (`UIF` in the `SR` register) is cleared manually to avoid re-triggering.

## Hardware Verification & Visuals

To verify the precision of the interrupt execution, the output waveforms were captured and analyzed using a hardware logic analyzer and **Logoc 2** open-source software. 

### Timing Diagrams
The captures confirm that the GPIO toggles precisely within the Interrupt Service Routine at the expected hardware-timed intervals:
*   **100 ms Mode:** Generates a precise 100 ms high/low pulse width (5 Hz frequency).
<img width="1873" height="463" alt="image" src="https://github.com/user-attachments/assets/3673b353-e1dd-40de-8f30-8b0882aea56b" />

