#include "gpio.h"
#include "stm32f103.h"

void GPIO_Init(void)
{
    /* Enable GPIOA clock */
    RCC->APB2ENR |= (1 << 2);

    /*
     * PA0 -> TIM2_CH1
     * PA1 -> TIM2_CH2
     * PA2 -> TIM2_CH3
     * PA3 -> TIM2_CH4
     *
     * Alternate Function Push-Pull
     * 50 MHz
     *
     * MODE = 11
     * CNF  = 10
     * => 1011 = 0xB
     */

    // PA0
    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |=  (0xB << 0);

    // PA1
    GPIOA->CRL &= ~(0xF << 4);
    GPIOA->CRL |=  (0xB << 4);

    // PA2
    GPIOA->CRL &= ~(0xF << 8);
    GPIOA->CRL |=  (0xB << 8);

    // PA3
    GPIOA->CRL &= ~(0xF << 12);
    GPIOA->CRL |=  (0xB << 12);
}