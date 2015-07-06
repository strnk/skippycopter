#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/usart.h>

#define CLI_NO_EXTERN
#include <cli/cli.h>
#include <config.h>

// Global initialization of the CLI handler
extern const cli_funcdesc_t cli_functions[];
CommandLineInterface CLIHandler(cli_functions);

// RX UART buffer
volatile char uart_debug_rx_buffer[CLI_CMD_LINE_MAXLEN+1];
volatile uint8_t uart_debug_rx_ptr = 0;

// Static allocation of ARGV buffer
char *cli_argv[CLI_CMD_LINE_MAXARGS];

CommandLineInterface::CommandLineInterface(const cli_funcdesc_t* funcPtr)
	: funcs(funcPtr), pending(false)
{

}

void 
CommandLineInterface::init(void)
{
	/* Setup UART parameters. */
	usart_set_baudrate(UART_DEBUG, 115200);
	usart_set_databits(UART_DEBUG, 8);
	usart_set_stopbits(UART_DEBUG, USART_STOPBITS_1);
	usart_set_mode(UART_DEBUG, USART_MODE_TX_RX);
	usart_set_parity(UART_DEBUG, USART_PARITY_NONE);
	usart_set_flow_control(UART_DEBUG, USART_FLOWCONTROL_NONE);

	/* Unmack the RX interrupt */
	usart_enable_rx_interrupt(UART_DEBUG);
	nvic_enable_irq(IRQ_UART_DEBUG);

	/* Finally enable the UART. */
	usart_enable(UART_DEBUG);
}

void
CommandLineInterface::prompt(void)
{
	// The BT module buffers the output, we need to add a return
	// for the prompt to appear on the host side
	printf("skp>\n");
}

uint8_t
CommandLineInterface::_argvize(char *pstr)
{
	uint8_t argc = 0;
	char *str = pstr;
	cli_argv[argc++] = str;

	while (*str != '\0')
	{
		if (*str == ' ') {
			cli_argv[argc++] = str+1;
			*str = '\0';
		}
		str++;
	}

	return argc;
}

void 
CommandLineInterface::trigger(void)
{
	uint8_t argc;
	const cli_funcdesc_t* fDesc = NULL;
	pending = false;
	uart_debug_rx_buffer[uart_debug_rx_ptr] = '\0';

	// Initialize arc and fill argv
	argc = _argvize((char*)uart_debug_rx_buffer);

	if (strcmp(cli_argv[0], "")) {
		for (int i = 0; funcs[i].funcptr != NULL; i++)
		{
			if (!strcmp(cli_argv[0], funcs[i].cmd))
				fDesc = &(funcs[i]);
		}

		if (fDesc == NULL) {
			printf("Unknown command `%s'.\n", cli_argv[0]);
		} else if (argc <= fDesc->req_args) {
			printf("`%s' requires at least %d parameters.\n", cli_argv[0], fDesc->req_args);
		} else {
			fDesc->funcptr(argc, cli_argv);
		}
	}

	uart_debug_rx_ptr = 0;
	prompt();
}

// UART_DEBUG interrupt
extern "C"
void 
usart_debug_isr(void)
{
	uint8_t data;

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(UART_DEBUG) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(UART_DEBUG) & USART_SR_RXNE) != 0)) {
		/* Retrieve the data from the peripheral. */
		data = usart_recv(UART_DEBUG);

		if (data == '\r' || data == '\n') {
			CLIHandler.pending = true;
		} else if (data == '\b') {
			uart_debug_rx_ptr--;
		} else if (uart_debug_rx_ptr < CLI_CMD_LINE_MAXLEN) {
			uart_debug_rx_buffer[uart_debug_rx_ptr++] = data;
		}
	}
}

// Use UART_DEBUG for write operations
extern "C"
int 
_write(int file, char *ptr, int len)
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