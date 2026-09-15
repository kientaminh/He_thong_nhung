#ifndef UART_H
#define UART_H

#include <stdint.h>

void USART1_Init(void);

void UART_SendChar(char c);
void UART_SendString(const char *str);
void UART_SendNumber(uint16_t num);

#endif
