#include <stdint.h>

#include "stm32f10x.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

/* PA0: button to GND. PA9: USART1 TX to the USB-UART adapter RX. */
#define BUTTON_PIN              (1UL << 0)
#define UART_BAUD_RATE          115200UL
#define BUTTON_DEBOUNCE_TICKS   pdMS_TO_TICKS(50)
#define COUNT_QUEUE_LENGTH      16U

static QueueHandle_t countQueue;
static uint32_t buttonCount;

static void UART1_Init(void)
{
    /* SystemInit configures HCLK and PCLK2 to 72 MHz in this project. */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_USART1EN;

    /* PA9: alternate-function push-pull, 50 MHz (CNF=10, MODE=11). */
    GPIOA->CRH &= ~(0xFUL << 4);
    GPIOA->CRH |=  (0xBUL << 4);

    USART1->BRR = (SystemCoreClock + UART_BAUD_RATE / 2UL) / UART_BAUD_RATE;
    USART1->CR1 = USART_CR1_TE | USART_CR1_UE;
}

static void Button_EXTI_Init(void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_AFIOEN;

    /* PA0 input with internal pull-up; pressing the button pulls it low. */
    GPIOA->CRL &= ~0xFUL;
    GPIOA->CRL |=  0x8UL;
    GPIOA->BSRR = BUTTON_PIN;

    /* Route EXTI0 to PA0 and trigger only on the falling edge. */
    AFIO->EXTICR[0] &= ~0xFUL;
    EXTI->IMR  &= ~BUTTON_PIN;
    EXTI->RTSR &= ~BUTTON_PIN;
    EXTI->FTSR |=  BUTTON_PIN;
    EXTI->PR = BUTTON_PIN;

    /* Priority 12 is below configMAX_SYSCALL_INTERRUPT_PRIORITY (11). */
    NVIC_SetPriority(EXTI0_IRQn, 12U);
    NVIC_ClearPendingIRQ(EXTI0_IRQn);
}

static void UART1_SendChar(char character)
{
    while ((USART1->SR & USART_SR_TXE) == 0U)
    {
    }

    USART1->DR = (uint16_t)character;
}

static void UART1_SendText(const char *text)
{
    while (*text != '\0')
    {
        UART1_SendChar(*text++);
    }
}

static void UART1_SendUnsigned(uint32_t value)
{
    char digits[10];
    uint32_t length = 0U;

    do
    {
        digits[length++] = (char)('0' + (value % 10U));
        value /= 10U;
    } while (value != 0U);

    while (length != 0U)
    {
        UART1_SendChar(digits[--length]);
    }
}

static void UART_Task(void *argument)
{
    uint32_t count;

    (void)argument;

    /* Enable the button interrupt only after the scheduler is running. */
    EXTI->PR = BUTTON_PIN;
    NVIC_EnableIRQ(EXTI0_IRQn);
    EXTI->IMR |= BUTTON_PIN;

    for (;;)
    {
        /* The task remains Blocked until EXTI0 sends a new count. */
        if (xQueueReceive(countQueue, &count, portMAX_DELAY) == pdPASS)
        {
            UART1_SendText("So lan nhan nut: ");
            UART1_SendUnsigned(count);
            UART1_SendText("\r\n");
        }
    }
}

void EXTI0_IRQHandler(void)
{
    static TickType_t lastPressTick;
    static BaseType_t hasPreviousPress = pdFALSE;
    BaseType_t higherPriorityTaskWoken = pdFALSE;
    TickType_t currentTick;

    if ((EXTI->PR & BUTTON_PIN) != 0U)
    {
        EXTI->PR = BUTTON_PIN; /* Write one to clear the pending flag. */
        currentTick = xTaskGetTickCountFromISR();

        if ((hasPreviousPress == pdFALSE) ||
            ((TickType_t)(currentTick - lastPressTick) >= BUTTON_DEBOUNCE_TICKS))
        {
            lastPressTick = currentTick;
            hasPreviousPress = pdTRUE;
            ++buttonCount;
            (void)xQueueSendFromISR(countQueue, &buttonCount,
                                    &higherPriorityTaskWoken);
        }

        portYIELD_FROM_ISR(higherPriorityTaskWoken);
    }
}

int main(void)
{
    UART1_Init();

    countQueue = xQueueCreate(COUNT_QUEUE_LENGTH, sizeof(uint32_t));
    if (countQueue == NULL)
    {
        for (;;)
        {
        }
    }

    if (xTaskCreate(UART_Task, "UART", 256U, NULL, 2U, NULL) != pdPASS)
    {
        for (;;)
        {
        }
    }

    Button_EXTI_Init();
    vTaskStartScheduler();

    for (;;)
    {
    }
}
