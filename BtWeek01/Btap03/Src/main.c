#include <stdint.h>
// RCC
#define RCC_CR          (*(volatile unsigned int *)0x40021000)
#define RCC_CFGR        (*(volatile unsigned int *)0x40021004)
#define RCC_APB2ENR     (*(volatile unsigned int *)0x40021018)

#define FLASH_ACR       (*(volatile unsigned int *)0x40022000)

#define GPIOA_CRL       (*(volatile unsigned int *)0x40010800)
#define GPIOA_BSRR      (*(volatile unsigned int *)0x40010810)
#define GPIOA_ODR       (*(volatile unsigned int *)0x4001080C)

#define GPIOB_CRH       (*(volatile unsigned int *)0x40010C04)
#define GPIOB_IDR       (*(volatile unsigned int *)0x40010C08)
#define GPIOB_BSRR      (*(volatile unsigned int *)0x40010C10)

#define SYST_CSR        (*(volatile unsigned int *)0xE000E010)
#define SYST_RVR        (*(volatile unsigned int *)0xE000E014)
#define SYST_CVR        (*(volatile unsigned int *)0xE000E018)


static void clock_init(void)
{
    // FLASH: 2 wait states
    FLASH_ACR &= ~0x7U;
    FLASH_ACR |=  0x2U;

    // Enable HSE
    RCC_CR |= (1U << 16);

    // Wait HSE ready
    while (!(RCC_CR & (1U << 17)))
    {
    }

    /*
     * APB1 = HCLK / 2 = 36 MHz
     * PLL source = HSE
     * PLL = x9
     */
    RCC_CFGR = 0;

    // APB1 prescaler /2
    RCC_CFGR |= (4U << 8);

    // PLL source = HSE
    RCC_CFGR |= (1U << 16);

    // PLL multiplier x9
    RCC_CFGR |= (7U << 18);

    // Enable PLL
    RCC_CR |= (1U << 24);

    // Wait PLL ready
    while (!(RCC_CR & (1U << 25)))
    {
    }

    // Select PLL as SYSCLK
    RCC_CFGR &= ~(3U << 0);
    RCC_CFGR |=  (2U << 0);

    // Wait until PLL is SYSCLK
    while ((RCC_CFGR & (3U << 2)) != (2U << 2))
    {
    }
}


static void systick_init(void)
{
    // 72 MHz / 1000 = 72000 cycles = 1 ms
    SYST_RVR = 72000U - 1U;

    SYST_CVR = 0;

    // ENABLE = 1
    // TICKINT = 0
    // CLKSOURCE = CPU clock
    SYST_CSR = (1U << 2) | (1U << 0);
}


static void delay_ms(unsigned int ms)
{
    while (ms--)
    {
        while (!(SYST_CSR & (1U << 16)))
        {
        }
    }
}


static void gpio_init(void)
{
    /*
     * Enable:
     * GPIOA clock = bit 2
     * GPIOB clock = bit 3
     */
    RCC_APB2ENR |= (1U << 2);
    RCC_APB2ENR |= (1U << 3);


    GPIOA_CRL = 0x22222222;


    
    GPIOB_CRH = 0x88888888;
    GPIOB_BSRR = 0x0000FF00;
}


int main(void)
{
    clock_init();
    systick_init();
    gpio_init();

    while (1)
    {
        uint32_t input;
        uint32_t output;

        input = (GPIOB_IDR >> 8) & 0xFF;

        output = input;

        GPIOA_ODR = (GPIOA_ODR & 0xFFFFFF00) | output;
    }
}