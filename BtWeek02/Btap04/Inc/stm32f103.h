#ifndef STM32F103_H
#define STM32F103_H

#include <stdint.h>

typedef struct
{
    volatile uint32_t CR;          // 0x00
    volatile uint32_t CFGR;        // 0x04
    volatile uint32_t CIR;         // 0x08
    volatile uint32_t APB2RSTR;    // 0x0C
    volatile uint32_t APB1RSTR;    // 0x10
    volatile uint32_t AHBENR;      // 0x14
    volatile uint32_t APB2ENR;     // 0x18
    volatile uint32_t APB1ENR;     // 0x1C
} RCC_TypeDef;

typedef struct
{
    volatile uint32_t ACR;         // 0x00
} FLASH_TypeDef;

typedef struct
{
    volatile uint32_t CRL;         // 0x00
    volatile uint32_t CRH;         // 0x04
    volatile uint32_t IDR;         // 0x08
    volatile uint32_t ODR;         // 0x0C
    volatile uint32_t BSRR;        // 0x10
    volatile uint32_t BRR;         // 0x14
    volatile uint32_t LCKR;        // 0x18
} GPIO_TypeDef;

typedef struct
{
    volatile uint32_t SR;          // 0x00
    volatile uint32_t CR1;         // 0x04
    volatile uint32_t CR2;         // 0x08
    volatile uint32_t SMPR1;       // 0x0C
    volatile uint32_t SMPR2;       // 0x10
    volatile uint32_t JOFR1;       // 0x14
    volatile uint32_t JOFR2;       // 0x18
    volatile uint32_t JOFR3;       // 0x1C
    volatile uint32_t JOFR4;       // 0x20
    volatile uint32_t HTR;         // 0x24
    volatile uint32_t LTR;         // 0x28
    volatile uint32_t SQR1;        // 0x2C
    volatile uint32_t SQR2;        // 0x30
    volatile uint32_t SQR3;        // 0x34
    volatile uint32_t JSQR;        // 0x38
    volatile uint32_t JDR1;        // 0x3C
    volatile uint32_t JDR2;        // 0x40
    volatile uint32_t JDR3;        // 0x44
    volatile uint32_t JDR4;        // 0x48
    volatile uint32_t DR;          // 0x4C
} ADC_TypeDef;

typedef struct
{
    volatile uint32_t CR1;       // 0x00
    volatile uint32_t CR2;       // 0x04
    volatile uint32_t SMCR;      // 0x08
    volatile uint32_t DIER;      // 0x0C
    volatile uint32_t SR;        // 0x10
    volatile uint32_t EGR;       // 0x14
    volatile uint32_t CCMR1;     // 0x18
    volatile uint32_t CCMR2;     // 0x1C
    volatile uint32_t CCER;      // 0x20
    volatile uint32_t CNT;       // 0x24
    volatile uint32_t PSC;       // 0x28
    volatile uint32_t ARR;       // 0x2C
    volatile uint32_t RESERVED1; // 0x30
    volatile uint32_t CCR1;      // 0x34
    volatile uint32_t CCR2;      // 0x38
    volatile uint32_t CCR3;      // 0x3C
    volatile uint32_t CCR4;      // 0x40
} TIM_TypeDef;

#define RCC_BASE        0x40021000UL
#define FLASH_BASE      0x40022000UL
#define GPIOA_BASE      0x40010800UL
#define USART1_BASE     0x40013800UL
#define ADC1_BASE       0x40012400UL
#define SYSTICK_BASE    0xE000E010UL
#define TIM2_BASE       0x40000000UL
#define TIM3_BASE       0x40000400UL

#define RCC             ((RCC_TypeDef *)RCC_BASE)
#define FLASH           ((FLASH_TypeDef *)FLASH_BASE)
#define GPIOA           ((GPIO_TypeDef *)GPIOA_BASE)
#define USART1          ((USART_TypeDef *)USART1_BASE)
#define ADC1            ((ADC_TypeDef *)ADC1_BASE)
#define TIM2            ((TIM_TypeDef *)TIM2_BASE)
#define TIM3            ((TIM_TypeDef *)TIM3_BASE)

#endif

