#include <errno.h>
#include <unistd.h>
#include <libopencm3/stm32/usart.h>
#include "debug.h"
#include "config.h"


int _write(int file, char *ptr, int len);

void 
debug_setup(void)
{
    // Enable UART clock
    rcc_periph_clock_enable(RCC_UART_DEBUG);

	/* Setup UART parameters. */
	usart_set_baudrate(UART_DEBUG, 9600);
	usart_set_databits(UART_DEBUG, 8);
	usart_set_stopbits(UART_DEBUG, USART_STOPBITS_1);
	usart_set_mode(UART_DEBUG, USART_MODE_TX);
	usart_set_parity(UART_DEBUG, USART_PARITY_NONE);
	usart_set_flow_control(UART_DEBUG, USART_FLOWCONTROL_NONE);

	/* Finally enable the UART. */
	usart_enable(UART_DEBUG);
}

// Use UART3 for write operations
int _write(int file, char *ptr, int len)
{
	int i;

	if (file == STDOUT_FILENO || file == STDERR_FILENO) {
		for (i = 0; i < len; i++) {
			if (ptr[i] == '\n') {
				usart_send_blocking(UART_DEBUG, '\r');
			}
			usart_send_blocking(UART_DEBUG, ptr[i]);
		}
		return i;
	}
	errno = EIO;
	return -1;
}