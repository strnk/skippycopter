#include <stdio.h>
#include <system/system.h>
#include <periph/ppm.h>
#include <sys/flags.h>
#include <string.h>
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

extern "C"
uint8_t
cli_func_system_flag_enable(int argc, char **argv)
{
	return setFlagValue(argv[1], 1);
}

extern "C"
uint8_t
cli_func_system_flag_disable(int argc, char **argv)
{
	return setFlagValue(argv[1], 0);
}

extern "C"
uint8_t
cli_func_system_ppm(int argc, char **argv)
{
	extern PPM ppm;

	printf("PPM values (%d invalid):\n", ppm.invalid_frames);
	for (int i = 0; i <= ppm.channels; i++)
		printf("%d: %ld\n", i, ppm.pulses[i]/6);
}