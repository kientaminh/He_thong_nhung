#include "clock.h"
#include "stm32f103.h"

void Clock_Init(void)
{
    /*
     * =========================
     * Enable HSE
     * =========================
     */

    // HSEON = 1
    RCC->CR |= (1 << 16);

    // Wait until HSERDY = 1
    while (!(RCC->CR & (1 << 17)));


    /*
     * =========================
     * Flash latency
     * =========================
     *
     * 72 MHz -> 2 wait states
     */

    FLASH->ACR &= ~(0x7);
    FLASH->ACR |=  (0x2);


    /*
     * =========================
     * AHB prescaler
     * =========================
     *
     * HCLK = SYSCLK
     *
     * HPRE = 0000
     */

    RCC->CFGR &= ~(0xF << 4);


    /*
     * =========================
     * APB1 prescaler
     * =========================
     *
     * APB1 max = 36 MHz
     *
     * PPRE1 = 100
     * HCLK / 2 = 36 MHz
     */

    RCC->CFGR &= ~(0x7 << 8);
    RCC->CFGR |=  (0x4 << 8);


    /*
     * =========================
     * APB2 prescaler
     * =========================
     *
     * APB2 = HCLK = 72 MHz
     *
     * PPRE2 = 000
     */

    RCC->CFGR &= ~(0x7 << 11);


    /*
     * =========================
     * PLL source
     * =========================
     *
     * PLL source = HSE
     *
     * PLLSRC = 1
     */

    RCC->CFGR |= (1 << 16);


    /*
     * =========================
     * PLL multiplier
     * =========================
     *
     * HSE = 8 MHz
     *
     * PLL = 8 MHz x 9
     *     = 72 MHz
     *
     * PLLMUL = 0111
     */

    RCC->CFGR &= ~(0xF << 18);
    RCC->CFGR |=  (0x7 << 18);


    /*
     * =========================
     * Enable PLL
     * =========================
     */

    RCC->CR |= (1 << 24);

    // Wait until PLLRDY = 1
    while (!(RCC->CR & (1 << 25)));


    /*
     * =========================
     * Select PLL as SYSCLK
     * =========================
     *
     * SW = 10
     */

    RCC->CFGR &= ~(0x3);
    RCC->CFGR |=  (0x2);


    /*
     * Wait until PLL is actually
     * selected as system clock
     *
     * SWS = 10
     */

    while (((RCC->CFGR >> 2) & 0x3) != 0x2);
}
