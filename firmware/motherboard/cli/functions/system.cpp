#include <stdio.h>
#include <system/system.h>
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

uint8_t
flag_set(char *name, uint8_t value)
{
	if (!strcmp(name, "data_reporting"))
		cli_flags.data_reporting = value;
	else if (!strcmp(name, "orientation_reporting"))
		cli_flags.orientation_reporting = value;
	else if (!strcmp(name, "raw_attitude"))
		cli_flags.raw_attitude = value;
	else if (!strcmp(name, "gps_proxy"))
		cli_flags.gps_proxy = value;
	else
		return 1;

	return 0;
}

extern "C"
uint8_t
cli_func_system_flag_enable(int argc, char **argv)
{
	return flag_set(argv[1], 1);
}

extern "C"
uint8_t
cli_func_system_flag_disable(int argc, char **argv)
{
	return flag_set(argv[1], 0);
}