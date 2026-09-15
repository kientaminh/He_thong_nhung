#include "delay.h"
#include "stm32f103.h"

void SysTick_Init(void)
{
    SysTick->LOAD = 72000 - 1;
    SysTick->VAL = 0;
    SysTick->CTRL = (1 << 2) | (1 << 0);
}

void Delay_ms(uint32_t ms)
{
    while (ms--)
    {
        while (!(SysTick->CTRL & (1 << 16)));
    }
}