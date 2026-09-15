#include <stdint.h>
#include "clock.h"
#include "delay.h"
#include "uart.h"
#include "adc.h"

int main(void)
{
    Clock_Init();
    SysTick_Init();
    USART1_Init();
    ADC1_Init();

    UART_SendString("System Ready\r\n");

    while (1)
    {
        uint16_t adc_value;
        uint32_t voltage_mV;
        uint16_t decimal;

        /* Read ADC */
        adc_value = ADC1_Read();

        /* Convert ADC value to voltage
         *
         * ADC: 0 ~ 4095
         * Vref: 3300 mV
         */
        voltage_mV = ((uint32_t)adc_value * 3300) / 4095;

        /* Send ADC value */
        UART_SendString("ADC = ");
        UART_SendNumber(adc_value);

        /* Send voltage */
        UART_SendString(" | Voltage = ");

        UART_SendNumber(voltage_mV / 1000);

        UART_SendChar('.');

        decimal = voltage_mV % 1000;

        if (decimal < 100)
        {
            UART_SendChar('0');
        }

        if (decimal < 10)
        {
            UART_SendChar('0');
        }

        UART_SendNumber(decimal);

        UART_SendString(" V\r\n");
        Delay_ms(1000);
    }
}
