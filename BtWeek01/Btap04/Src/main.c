// RCC
#define RCC_CR          (*(volatile unsigned int *)0x40021000)
#define RCC_CFGR        (*(volatile unsigned int *)0x40021004)
#define RCC_APB2ENR     (*(volatile unsigned int *)0x40021018)

// FLASH
#define FLASH_ACR       (*(volatile unsigned int *)0x40022000)

// GPIOA
#define GPIOA_CRL       (*(volatile unsigned int *)0x40010800)
#define GPIOA_BSRR      (*(volatile unsigned int *)0x40010810)

// GPIOB
#define GPIOB_CRH       (*(volatile unsigned int *)0x40010C04)
#define GPIOB_IDR       (*(volatile unsigned int *)0x40010C08)
#define GPIOB_BSRR      (*(volatile unsigned int *)0x40010C10)

// SysTick
#define SYST_CSR        (*(volatile unsigned int *)0xE000E010)
#define SYST_RVR        (*(volatile unsigned int *)0xE000E014)
#define SYST_CVR        (*(volatile unsigned int *)0xE000E018)

/*
 * GPIO:
 *
 * PA0 = Output push-pull 2 MHz
 * PB8 = Input pull-up
 */
static void gpio_init(void)
{
    // Enable GPIOA clock
    RCC_APB2ENR |= (1U << 2);

    // Enable GPIOB clock
    RCC_APB2ENR |= (1U << 3);


    /*
     * PA0: Output push-pull 2 MHz
     *
     * PA0 nằm trong CRL, bits [3:0]
     *
     * MODE0 = 10 -> Output 2 MHz
     * CNF0  = 00 -> General purpose push-pull
     *
     * => 0010 = 0x2
     */
    GPIOA_CRL &= ~(0xFU << 0);
    GPIOA_CRL |=  (0x2U << 0);


    /*
     * PA0 = 0 ban đầu
     */
    GPIOA_BSRR = (1U << (0 + 16));


    /*
     * PB8: Input pull-up
     *
     * PB8 nằm trong CRH, bits [3:0]
     *
     * MODE8 = 00
     * CNF8  = 10
     *
     * => 1000 = 0x8
     */
    GPIOB_CRH &= ~(0xFU << 0);
    GPIOB_CRH |=  (0x8U << 0);


    /*
     * Pull-up nội:
     *
     * PB8 ODR = 1
     */
    GPIOB_BSRR = (1U << 8);
}
/*
 * PB8 = 0 -> nút nhấn
 * PB8 = 1 -> nút thả
 */
static unsigned char button_pressed(void)
{
    return (GPIOB_IDR & (1U << 8)) == 0;
}
static void led_on(void)
{
    GPIOA_BSRR = (1U << 0);
}

static void led_off(void)
{
    GPIOA_BSRR = (1U << (0 + 16));
}
int main(void)
{
    clock_init();
    systick_init();
    gpio_init();

    while (1)
    {
        if (button_pressed())
        {
            // Debounce
            delay_ms(20);

            if (button_pressed())
            {
                led_on();

                // Giữ nút → LED tiếp tục sáng
                while (button_pressed())
                {
                }

                // Debounce lúc nhả
                delay_ms(20);

                led_off();
            }
        }
        else
        {
            led_off();
        }
    }
}