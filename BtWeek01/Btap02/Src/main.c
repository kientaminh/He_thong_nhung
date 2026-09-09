// RCC
#define RCC_CR          (*(volatile unsigned int *)0x40021000)
#define RCC_CFGR        (*(volatile unsigned int *)0x40021004)
#define RCC_APB2ENR     (*(volatile unsigned int *)0x40021018)

// FLASH
#define FLASH_ACR       (*(volatile unsigned int *)0x40022000)

// GPIOA
#define GPIOA_CRL       (*(volatile unsigned int *)0x40010800)
#define GPIOA_BSRR      (*(volatile unsigned int *)0x40010810)

// SysTick
#define SYST_CSR        (*(volatile unsigned int *)0xE000E010)
#define SYST_RVR        (*(volatile unsigned int *)0xE000E014)
#define SYST_CVR        (*(volatile unsigned int *)0xE000E018)


/* 
 * CLOCK: 8 MHz HSE - 72 MHz SYSCLK
 */
static void clock_init(void)
{
    /*
     * 72 MHz cần 2 wait states cho FLASH
     *
     * LATENCY[2:0] = 010
     */
    FLASH_ACR &= ~0x7U;
    FLASH_ACR |=  0x2U;


    /*
     * Bật HSE
     *
     * RCC_CR bit 16 = HSEON
     */
    RCC_CR |= (1U << 16);

    /*
     * Chờ HSE ổn định
     *
     * RCC_CR bit 17 = HSERDY
     */
    while (!(RCC_CR & (1U << 17)))
    {
    }


    /*
     * Cấu hình:
     *
     * AHB  = SYSCLK / 1 = 72 MHz
     * APB1 = HCLK / 2   = 36 MHz
     * APB2 = HCLK / 1   = 72 MHz
     *
     * PLL source = HSE
     * PLL multiplier = x9
     */

    RCC_CFGR = 0;

    /* APB1 prescaler = /2
       PPRE1 = 100
    */
    RCC_CFGR |= (4U << 8);

    /* PLL source = HSE */
    RCC_CFGR |= (1U << 16);

    /* PLL multiplier = x9
       PLLMUL = 0111
    */
    RCC_CFGR |= (7U << 18);


    /*
     * Bật PLL
     */
    RCC_CR |= (1U << 24);

    /*
     * Chờ PLL lock
     */
    while (!(RCC_CR & (1U << 25)))
    {
    }


    /*
     * Chọn PLL làm SYSCLK
     *
     * SW = 10
     */
    RCC_CFGR &= ~(3U << 0);
    RCC_CFGR |=  (2U << 0);


    /*
     * Chờ hệ thống chuyển sang PLL
     *
     * SWS = 10
     */
    while ((RCC_CFGR & (3U << 2)) != (2U << 2))
    {
    }
}


/* 
 * SysTick
 */
static void systick_init(void)
{
    /*
     * CPU = 72 MHz
     *
     * 1 ms:
     *
     * 72,000,000 / 1000 = 72,000
     *
     * Reload = 72000 - 1
     */
    SYST_RVR = 72000U - 1U;

    /* Clear current value */
    SYST_CVR = 0;

    /*
     * bit 0 ENABLE    = 1
     * bit 1 TICKINT   = 0   (không dùng interrupt)
     * bit 2 CLKSOURCE = 1   (CPU clock = 72 MHz)
     */
    SYST_CSR = (1U << 2) | (1U << 0);
}


/* 
 * Delay 
 */
static void delay_ms(unsigned int ms)
{
    while (ms--)
    {
        /*
         * COUNTFLAG = bit 16
         * được set khi SysTick đếm từ 0 về reload
         */
        while (!(SYST_CSR & (1U << 16)))
        {
        }
    }
}

static void gpio_init(void)
{
    // Enable GPIOA clock
    RCC_APB2ENR |= (1U << 2);

    /*
     * PA0 -> PA7 nằm trong CRL
     *
     * Mỗi pin chiếm 4 bit:
     *
     * PA0: [3:0]
     * PA1: [7:4]
     * PA2: [11:8]
     * ...
     * PA7: [31:28]
     */

    GPIOA_CRL = 0x22222222;
}

static void led_off_all(void)
{
    GPIOA_BSRR = 0x000000FF;
}

static void led_on(unsigned int pin)
{
    GPIOA_BSRR = (1U << (pin + 16));
}



int main(void)
{
    clock_init();
    systick_init();
    gpio_init();
    led_off_all();

    while (1)
    {
        //Nháy Led từ phải sang trái
        for (int i = 0; i < 8; i++)
        {
            led_off_all();

            led_on(i);

            delay_ms(200);
        }
        //Nháy Led từ trái sang phải
        for (int i = 6; i >= 0; i--)
        {
            led_off_all();

            led_on(i);

            delay_ms(200);
        }
    }
}