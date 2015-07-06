#include <stdio.h>
#include "system.h"

uint8_t 
cli_func_system_ping(int argc, char **argv)
{
	puts("Pong!");
	return 0;
}