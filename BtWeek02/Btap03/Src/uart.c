#include "uart.h"
#include "stm32f103.h"

void USART1_Init(void)
{
    // Enable GPIOA clock
    RCC->APB2ENR |= (1 << 2);

    // Enable USART1 clock
    RCC->APB2ENR |= (1 << 14);

    /*
     * PA9 = USART1_TX
     * Output: Alternate Function Push-Pull
     * MODE = 11
     * CNF  = 10
     * => 0xB
     */
    GPIOA->CRH &= ~(0xF << 4);
    GPIOA->CRH |=  (0xB << 4);

    /*
     * PA10 = USART1_RX
     * Input Floating
     * MODE = 00
     * CNF  = 01
     * => 0x4
     */
    GPIOA->CRH &= ~(0xF << 8);
    GPIOA->CRH |=  (0x4 << 8);

    /*
     * Baud rate:
     * USART clock = 72 MHz
     * Baud rate   = 115200
     *
     * BRR = 0x271
     */
    USART1->BRR = 0x271;

    /*
     * UE  = 1 -> Enable USART
     * TE  = 1 -> Enable transmitter
     * RE  = 1 -> Enable receiver
     */
    USART1->CR1 = (1 << 13) |
                  (1 << 3)  |
                  (1 << 2);
}


void UART_SendChar(char c)
{
    // Wait until transmit data register is empty
    while (!(USART1->SR & (1 << 7)));

    USART1->DR = c;
}


void UART_SendString(const char *str)
{
    while (*str)
    {
        UART_SendChar(*str);
        str++;
    }
}


void UART_SendNumber(uint16_t num)
{
    char buffer[6];
    int i = 0;

    if (num == 0)
    {
        UART_SendChar('0');
        return;
    }

    // Convert number to ASCII
    while (num > 0)
    {
        buffer[i++] = '0' + (num % 10);
        num /= 10;
    }

    // Send in reverse order
    while (i > 0)
    {
        UART_SendChar(buffer[--i]);
    }
}

