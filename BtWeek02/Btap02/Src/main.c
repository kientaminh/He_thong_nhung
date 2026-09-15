#include <stdint.h>

typedef struct
{
    volatile uint32_t CR;        // 0x00
    volatile uint32_t CFGR;      // 0x04
    volatile uint32_t CIR;       // 0x08
    volatile uint32_t APB2RSTR;  // 0x0C
    volatile uint32_t APB1RSTR;  // 0x10
    volatile uint32_t AHBENR;    // 0x14
    volatile uint32_t APB2ENR;   // 0x18
    volatile uint32_t APB1ENR;   // 0x1C
} RCC_TypeDef;

typedef struct
{
    volatile uint32_t ACR;
} FLASH_TypeDef;

typedef struct
{
    volatile uint32_t CRL;   // 0x00
    volatile uint32_t CRH;   // 0x04
    volatile uint32_t IDR;   // 0x08
    volatile uint32_t ODR;   // 0x0C
    volatile uint32_t BSRR;  // 0x10
    volatile uint32_t BRR;   // 0x14
    volatile uint32_t LCKR;  // 0x18
} GPIO_TypeDef;

typedef struct
{
    volatile uint32_t CTRL;    // 0x00
    volatile uint32_t LOAD;    // 0x04
    volatile uint32_t VAL;     // 0x08
    volatile uint32_t CALIB;   // 0x0C
} SysTick_Type;

#define RCC ((RCC_TypeDef *)0x40021000)

#define FLASH ((FLASH_TypeDef *)0x40022000)

#define GPIOA ((GPIO_TypeDef *)0x40010800)
#define GPIOB ((GPIO_TypeDef *)0x40010C00)
#define GPIOC ((GPIO_TypeDef *)0x40011000)

#define SysTick ((SysTick_Type *)0xE000E010)

volatile uint32_t ms = 0;

void Clock_Init(void)
{
    // 1. Enable HSE
    RCC->CR |= (1 << 16);

    // 2. Wait HSE ready
    while (!(RCC->CR & (1 << 17)));

    // 3. Flash latency = 2 wait states
    FLASH->ACR &= ~(0x7);
    FLASH->ACR |= 0x2;

    // 4. AHB = SYSCLK / 1
    RCC->CFGR &= ~(0xF << 4);

    // 5. APB1 = HCLK / 2 = 36 MHz
    RCC->CFGR &= ~(0x7 << 8);
    RCC->CFGR |=  (0x4 << 8);

    // 6. APB2 = HCLK / 1 = 72 MHz
    RCC->CFGR &= ~(0x7 << 11);

    // 7. PLL source = HSE
    RCC->CFGR |= (1 << 16);

    // 8. PLL multiplier = x9
    RCC->CFGR &= ~(0xF << 18);
    RCC->CFGR |=  (0x7 << 18);

    // 9. Enable PLL
    RCC->CR |= (1 << 24);

    // 10. Wait PLL ready
    while (!(RCC->CR & (1 << 25)));

    // 11. Select PLL as SYSCLK
    RCC->CFGR &= ~(0x3 << 0);
    RCC->CFGR |=  (0x2 << 0);

    // 12. Wait until PLL is SYSCLK
    while (((RCC->CFGR >> 2) & 0x3) != 0x2);
}

void SysTick_Init(void)
{
    SysTick->LOAD = 72000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = (1 << 2) | (1 << 0) | (1 << 1);
}

void SysTick_Handler(void)
{
    ms++;
}

void GPIOA_Init(void)
{
    // Enable clock GPIOA
    RCC->APB2ENR |= (1 << 2);

    // PA0
    GPIOA->CRL &= ~(0xF << 0);
    GPIOA->CRL |=  (0x2 << 0);

    // PA1
    GPIOA->CRL &= ~(0xF << 4);
    GPIOA->CRL |=  (0x2 << 4);

    // PA2
    GPIOA->CRL &= ~(0xF << 8);
    GPIOA->CRL |=  (0x2 << 8);
}

int main(void)
{
    Clock_Init();
    SysTick_Init();
    GPIOA_Init();
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint32_t t2 = 0;
    
    while (1)
    {
        if(ms - t0 >= 500)
        {
            t0 = ms;
            GPIOA->ODR ^= (1 << 0);
        }
        if(ms - t1 >= 50)
        {
            t1 = ms;
            GPIOA->ODR ^= (1 << 1);
        }
        if(ms - t2 >= 5000)
        {
            t2 = ms;
            GPIOA->ODR ^= (1 << 2);
        }
    }
}