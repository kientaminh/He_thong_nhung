#include "clock.h"
#include "gpio.h"
#include "timer.h"

int main(void)
{
    Clock_Init();

    GPIO_Init();
    TIM2_Init();

    while (1)
    {
    }
}