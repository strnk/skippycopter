#include <stddef.h>
#include <cli/cli.h>

#include "functions/system.h"
#include "functions/pwm.h"

const cli_funcdesc_t cli_functions[] = {
	// System functions
	{ "ping",	0,	cli_func_system_ping },
	{ "power",	0, 	cli_func_system_power },

	// PWM functions
	{ "pwmlo",	1,	cli_func_pwm_pwmlo },
	{ "pwmhi",	1,	cli_func_pwm_pwmhi },

	// Empty record
	{ "xxx",		0, NULL }
};