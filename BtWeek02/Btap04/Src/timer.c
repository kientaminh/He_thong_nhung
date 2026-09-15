#include "timer.h"
#include "stm32f103.h"

void TIM2_Init(void)
{
    // Enable TIM2 clock
    RCC->APB1ENR |= (1 << 0);

    // Timer clock = 72 MHz
    // 72 MHz / 72 = 1 MHz
    TIM2->PSC = 71;

    // 1 MHz / 1000 = 1 kHz
    TIM2->ARR = 999;

    // Duty cycle
    TIM2->CCR1 = 100;   // 10%
    TIM2->CCR2 = 300;   // 30%
    TIM2->CCR3 = 500;   // 50%
    TIM2->CCR4 = 700;   // 70%

    // CH1 - PWM Mode 1
    TIM2->CCMR1 &= ~(0x7 << 4);
    TIM2->CCMR1 |=  (0x6 << 4);
    TIM2->CCMR1 |=  (1 << 3);

    // CH2 - PWM Mode 1
    TIM2->CCMR1 &= ~(0x7 << 12);
    TIM2->CCMR1 |=  (0x6 << 12);
    TIM2->CCMR1 |=  (1 << 11);

    // CH3 - PWM Mode 1
    TIM2->CCMR2 &= ~(0x7 << 4);
    TIM2->CCMR2 |=  (0x6 << 4);
    TIM2->CCMR2 |=  (1 << 3);

    // CH4 - PWM Mode 1
    TIM2->CCMR2 &= ~(0x7 << 12);
    TIM2->CCMR2 |=  (0x6 << 12);
    TIM2->CCMR2 |=  (1 << 11);

    // Enable CH1, CH2, CH3, CH4
    TIM2->CCER |= (1 << 0);
    TIM2->CCER |= (1 << 4);
    TIM2->CCER |= (1 << 8);
    TIM2->CCER |= (1 << 12);

    // Auto reload preload
    TIM2->CR1 |= (1 << 7);

    // Update event
    TIM2->EGR |= (1 << 0);

    // Start TIM2
    TIM2->CR1 |= (1 << 0);
}