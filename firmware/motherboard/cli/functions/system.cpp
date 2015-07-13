#include <stdio.h>
#include <system/system.h>
#include "system.h"

extern System sys;

extern "C"
uint8_t 
cli_func_system_ping(int argc, char **argv)
{
	puts("Pong!");
	return 0;
}

extern "C"
uint8_t
cli_func_system_power(int argc, char **argv)
{
	printf("V= %.3fV (avg cell: %.3f)\n", sys.powerboard.voltage_avg, sys.powerboard.voltage_avg/4);
	printf("I= %.3fA\n", sys.powerboard.intensity_avg);
}