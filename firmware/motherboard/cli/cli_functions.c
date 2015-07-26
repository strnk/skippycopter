#include <stddef.h>
#include <cli/cli.h>

#include "functions/system.h"
#include "functions/pwm.h"

const cli_funcdesc_t cli_functions[] = {
	// System functions
	{ "ping",	0,	cli_func_system_ping },
	{ "power",	0, 	cli_func_system_power },
	{ "fe",	1, 	cli_func_system_flag_enable },
	{ "fd",	1, 	cli_func_system_flag_disable },

	// PWM functions
	{ "pwmlo",	1,	cli_func_pwm_pwmlo },
	{ "pwmhi",	1,	cli_func_pwm_pwmhi },

	// PPM functions
	{ "ppm",	0, cli_func_system_ppm },

	// Empty record
	{ "xxx",		0, NULL }
};