#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <libopencm3/stm32/dbgmcu.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>


int _write(int file, char *ptr, int len);

/* Set STM32 to 168 MHz. */
static void clock_setup(void)
{
	rcc_clock_setup_hse_3v3(&hse_25mhz_3v3[CLOCK_3V3_168MHZ]);

	/* Enable GPIOB clock. */
	rcc_periph_clock_enable(RCC_GPIOB);
	
	rcc_periph_clock_enable(RCC_USART3);
}

static void usart_setup(void)
{
	/* Setup USART3 parameters. */
	usart_set_baudrate(USART3, 9600);
	usart_set_databits(USART3, 8);
	usart_set_stopbits(USART3, USART_STOPBITS_1);
	usart_set_mode(USART3, USART_MODE_TX);
	usart_set_parity(USART3, USART_PARITY_NONE);
	usart_set_flow_control(USART3, USART_FLOWCONTROL_NONE);

	/* Finally enable the USART. */
	usart_enable(USART3);
}

static void gpio_setup(void)
{
	/* Set GPIO12-15 (in GPIO port D) to 'output push-pull'. */
	gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT,
			GPIO_PUPD_NONE, GPIO12 | GPIO13);
			
	/* PB10/11 as UART3 TX/RX */
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO10);
	gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO11);
	gpio_set_af(GPIOB, GPIO_AF7, GPIO10);
	gpio_set_af(GPIOB, GPIO_AF7, GPIO11);
}

// Use UART3 for write operations
int _write(int file, char *ptr, int len)
{
	int i;

	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		for (i = 0; i < len; i++) {
			if (ptr[i] == '\n') {
				usart_send_blocking(USART3, '\r');
			}
			usart_send_blocking(USART3, ptr[i]);
		}
		return i;
	}
	errno = EIO;
	return -1;
}

int main(void)
{
	int i;

	clock_setup();
	gpio_setup();
	usart_setup();

	/* Set two LEDs for wigwag effect when toggling. */
	gpio_set(GPIOB, GPIO12);
	
	printf("Skippycopter v1.0\n");

	/* Blink the LEDs (PD12, PD13, PD14 and PD15) on the board. */
	while (1) {
		/* Toggle LEDs. */
		gpio_toggle(GPIOB, GPIO12 | GPIO13);
		printf(".");
		for (i = 0; i < 6000000; i++) { /* Wait a bit. */
			__asm__("nop");
		}
	}

	return 0;
}
