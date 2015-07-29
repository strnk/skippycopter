#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>

#define GPS_NO_EXTERN
#include <gps/gps.h>
#include <sys/flags.h>
#include <config.h>


GPS GPSHandler;


char uart_gps_rx_buffer0[GPS_NMEA_MSG_MAXLEN+1];
char uart_gps_rx_buffer1[GPS_NMEA_MSG_MAXLEN+1];

char *uart_gps_rx_read_buffer, *uart_gps_rx_write_buffer = uart_gps_rx_buffer0;
uint8_t uart_gps_rx_ptr = 0;

GPS::GPS(void)
	: pending(false)
{

}

void
GPS::init(void)
{
	/* Setup UART parameters. */
	usart_set_baudrate(UART_GPS, 9600);
	usart_set_databits(UART_GPS, 8);
	usart_set_stopbits(UART_GPS, USART_STOPBITS_1);
	usart_set_mode(UART_GPS, USART_MODE_TX_RX);
	usart_set_parity(UART_GPS, USART_PARITY_NONE);
	usart_set_flow_control(UART_GPS, USART_FLOWCONTROL_NONE);

	/* Unmack the RX interrupt */
	usart_enable_rx_interrupt(UART_GPS);
	nvic_enable_irq(IRQ_UART_GPS);

	/* Finally enable the UART. */
	usart_enable(UART_GPS);
}

void
GPS::trigger(void)
{
	pending = false;

	if (FLAG(msg_gps))
		printf("$GPS %s\n", uart_gps_rx_read_buffer);
}

extern "C"
void 
usart_gps_isr(void)
{
	uint8_t data;

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(UART_GPS) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(UART_GPS) & USART_SR_RXNE) != 0)) {
		/* Retrieve the data from the peripheral. */
		data = usart_recv(UART_GPS);

		if (data == '\r') {
			// ignore
		} else if (data == '\n') {
			uart_gps_rx_write_buffer[uart_gps_rx_ptr] = '\0';
			uart_gps_rx_read_buffer = uart_gps_rx_write_buffer;
			uart_gps_rx_ptr = 0;

			if (uart_gps_rx_write_buffer == uart_gps_rx_buffer0)
				uart_gps_rx_write_buffer = uart_gps_rx_buffer1;
			else
				uart_gps_rx_write_buffer = uart_gps_rx_buffer0;

			GPSHandler.pending = true;
		} else if (uart_gps_rx_ptr < GPS_NMEA_MSG_MAXLEN) {
			uart_gps_rx_write_buffer[uart_gps_rx_ptr++] = data;
		}
	}
}
