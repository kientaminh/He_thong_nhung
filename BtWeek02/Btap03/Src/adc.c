#include "adc.h"
#include "stm32f103.h"

void ADC1_Init(void)
{
    /* =========================
     * Enable GPIOA clock
     * ========================= */
    RCC->APB2ENR |= (1 << 2);

    /* =========================
     * Enable ADC1 clock
     * ========================= */
    RCC->APB2ENR |= (1 << 9);

    /* =========================
     * PA0 = ADC1_IN0
     * Analog input
     * CRL[3:0] = 0000
     * ========================= */
    GPIOA->CRL &= ~(0xF << 0);

    /* =========================
     * ADC clock
     *
     * 72 MHz / 6 = 12 MHz
     *
     * ADCPRE = 10
     * ========================= */
    RCC->CFGR &= ~(0x3 << 14);
    RCC->CFGR |=  (0x2 << 14);

    /* =========================
     * Regular sequence length
     *
     * L = 0
     * => 1 conversion
     * ========================= */
    ADC1->SQR1 &= ~(0xF << 20);

    /* =========================
     * First conversion
     *
     * Channel 0
     * ========================= */
    ADC1->SQR3 &= ~(0x1F << 0);

    /* =========================
     * Sampling time
     *
     * Channel 0
     * 55.5 ADC cycles
     * ========================= */
    ADC1->SMPR2 &= ~(0x7 << 0);
    ADC1->SMPR2 |=  (0x5 << 0);

    /* =========================
     * Select SWSTART
     *
     * EXTSEL = 111
     * ========================= */
    ADC1->CR2 &= ~(0x7 << 17);
    ADC1->CR2 |=  (0x7 << 17);

    /* Enable external trigger */
    ADC1->CR2 |= (1 << 20);

    /* =========================
     * Enable ADC
     * ========================= */
    ADC1->CR2 |= (1 << 0);

    /* Small delay */
    for (volatile uint32_t i = 0; i < 10000; i++);

    /* =========================
     * Reset calibration
     * ========================= */
    ADC1->CR2 |= (1 << 3);

    while (ADC1->CR2 & (1 << 3));

    /* =========================
     * Start calibration
     * ========================= */
    ADC1->CR2 |= (1 << 2);

    while (ADC1->CR2 & (1 << 2));
}


uint16_t ADC1_Read(void)
{
    /* =========================
     * Start ADC conversion
     * SWSTART = 1
     * ========================= */
    ADC1->CR2 |= (1 << 22);

    /* =========================
     * Wait for conversion complete
     *
     * EOC = 1
     * ========================= */
    while (!(ADC1->SR & (1 << 1)));

    /* =========================
     * Read ADC result
     *
     * 12-bit: 0 ~ 4095
     * ========================= */
    return (uint16_t)ADC1->DR;
}

