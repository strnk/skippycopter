#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include "board_setup.h"
#include "config.h"

/* Set STM32 to 168 MHz. */
void clock_setup(void)
{
    rcc_clock_setup_hse_3v3(&hse_25mhz_3v3[CLOCK_3V3_168MHZ]);
}

void gpio_setup(void)
{
    // Enable GPIO clocks
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_GPIOB);
    rcc_periph_clock_enable(RCC_GPIOC);
			 
    /* PA0/1/2/3 as TIM2 CH1/2/3/4 */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO0 | GPIO1 | GPIO2 | GPIO3);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_PP, GPIO_OSPEED_50MHZ, GPIO0 | GPIO1 | GPIO2 | GPIO3);
    gpio_set_af(GPIOA, GPIO_AF1, GPIO0 | GPIO1 | GPIO2 | GPIO3);

    /* PA8 as I2C3 SCL */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO8);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO8);
    gpio_set_af(GPIOA, GPIO_AF4, GPIO8);

    /* PA9/10 as USART1 TX/RX */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO9 | GPIO10);

    /* PB1 as TIM3 CH4 */
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO1);
    gpio_set_af(GPIOB, GPIO_AF2, GPIO1);

    /* PB6/7 as I2C1 SCL/SDA */
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);
    gpio_set_output_options(GPIOB, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO6 | GPIO7);
    gpio_set_af(GPIOB, GPIO_AF4, GPIO6 | GPIO7);

	/* PB10/11 as USART3 TX/RX */
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
	gpio_set_af(GPIOB, GPIO_AF7, GPIO10 | GPIO11);

    /* PB12/13 as LED1/LED2 */
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12 | GPIO13);

    /* PB14/15 as US_TRIGGER/US_ECHO */
    gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO15);

    /* PC6/7/8 as TIM3 CH1/2/3 */
    gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7 | GPIO8);
    gpio_set_af(GPIOC, GPIO_AF2, GPIO6 | GPIO7 | GPIO8);

    /* PC9 as I2C3 SDA */
    gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO9);
    gpio_set_output_options(GPIOC, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO9);
    gpio_set_af(GPIOC, GPIO_AF4, GPIO9);

    /* PC10/11 as UART4 TX/RX */
    gpio_mode_setup(GPIOC, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10 | GPIO11);
    gpio_set_af(GPIOC, GPIO_AF8, GPIO10 | GPIO11);

    // Enable timers clocks
    rcc_periph_clock_enable(RCC_PWM_LO_TIMER);
    rcc_periph_clock_enable(RCC_PWM_HI_TIMER);
}