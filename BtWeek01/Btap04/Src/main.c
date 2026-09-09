// RCC
#define RCC_CR          (*(volatile unsigned int *)0x40021000)
#define RCC_CFGR        (*(volatile unsigned int *)0x40021004)
#define RCC_APB2ENR     (*(volatile unsigned int *)0x40021018)

// FLASH
#define FLASH_ACR       (*(volatile unsigned int *)0x40022000)

// GPIOA
#define GPIOA_CRL       (*(volatile unsigned int *)0x40010800)
#define GPIOA_CRH       (*(volatile unsigned int *)0x40010804)
#define GPIOA_IDR       (*(volatile unsigned int *)0x40010808)
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
     * LATENCY[2:0] = 010
     */
    FLASH_ACR &= ~0x7U;
    FLASH_ACR |=  0x2U;


    /*
     * Bật HSE
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

/*
 * GPIO:   
 * PA0 = Input pull-up 
 * PA8 = Output push-pull 2 MHz 
 */ 
static void gpio_init(void) { 
    // Enable GPIOA clock 
    RCC_APB2ENR |= (1U << 2); 
    /*
     * PA0: Input pull-up  
     * PA0 nằm trong CRL, bits [3:0] 
     * MODE0 = 00 
     * CNF0 = 10 
     * => 0b1000 = 0x8 
     */ 
    GPIOA_CRL &= ~(0xFU << 0); 
    GPIOA_CRL |= (0x8U << 0); 
    /*
     * Kéo PA0 lên VCC bằng pull-up nội.
     * Với input pull-up: * ODR bit 0 = 1 
     */ 
    GPIOA_BSRR = (1U << 0); 
    /*
     * PA8: Output push-pull 2 MHz
     * PA8 nằm trong CRH, bits [3:0]
     * MODE8 = 10 -> output 2 MHz
     * CNF8 = 00 -> general purpose push-pull
     * => 0b0010 = 0x2
     */
    GPIOA_CRH &= ~(0xFU << 0);
    GPIOA_CRH |= (0x2U << 0);
    /*
     * Ban đầu LED OFF
     */
    GPIOA_BSRR = (1U << (8 + 16));
}

/*
 * Đọc trạng thái PA0 
 * PA0 = 0 -> nút đang nhấn 
 * PA0 = 1 -> nút đang thả 
 */ 
static unsigned char button_pressed(void) 
{ 
    return (GPIOA_IDR & (1U << 0)) == 0; 
} 
/*
 * Đảo trạng thái LED PA8 
 */ 
static void led_toggle(void) 
{ 
    static unsigned char led_state = 0; 
    led_state = !led_state; 
    if (led_state) 
    { 
        // PA8 = 1 
        GPIOA_BSRR = (1U << 8); 
    } 
    else 
    { 
        // PA8 = 0 
        GPIOA_BSRR = (1U << (8 + 16)); 
    } 
}


int main(void)
{
    clock_init();
    systick_init();
    gpio_init();

    while (1)
    {
        /* 
         * Chờ nút được nhấn 
         */ 
        if (button_pressed()) 
        { 
            /*
             * Debounce: 
             * Chờ 20 ms để loại bỏ rung phím 
             */ 
            delay_ms(20); 
             
            if (button_pressed()) 
            {
                while (button_pressed()) {} 
                /*
                 * Debounce lúc nhả 
                 */ 
                delay_ms(20); 
                if (!button_pressed()) 
                { 
                    led_toggle(); 
                } 
            } 
        }
    }
}