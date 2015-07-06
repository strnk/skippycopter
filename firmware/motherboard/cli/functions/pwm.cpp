#include <stdio.h>
#include <string.h>
#include <config.h>
#include "pwm.h"

#include <periph/pwm.h>

extern PWM pwm_lo;
extern PWM pwm_hi;

uint8_t 
pwm_set_throttle(PWM pwm, uint8_t channel, uint16_t throttle)
{
	tim_oc_id oc_channel;

	switch (channel)
	{
		case 1: oc_channel = PWM1_CHANNEL; break;
		case 2: oc_channel = PWM2_CHANNEL; break;
		case 3: oc_channel = PWM3_CHANNEL; break;
		case 4: oc_channel = PWM4_CHANNEL; break;
		default: return 1;
	}

	pwm.set_pulse_width(oc_channel, throttle);
	return 0;
}

extern "C"
uint8_t 
cli_func_pwm_pwmhi(int argc, char **argv)
{
	if (!strcmp(argv[1], "set")) {
		uint8_t channel;
		uint16_t throttle;

		if (argc <= 3) {
			puts("pwmhi set <channel> <throttle>");
			return 1;
		}

		channel = atoi(argv[2]);
		throttle = atoi(argv[3]);

		if (channel < 1 || channel > 4) {
			printf("Channel should be 1-4.\n");
			return 1;
		}

		return pwm_set_throttle(pwm_hi, channel, throttle);
	} else {
		printf("Unrecognized action `%s'\n", argv[1]);
	}

	return 0;
}

extern "C"
uint8_t 
cli_func_pwm_pwmlo(int argc, char **argv)
{
	if (!strcmp(argv[1], "set")) {
		uint8_t channel;
		uint16_t throttle;

		if (argc <= 3) {
			puts("pwmlo set <channel> <throttle>");
			return 1;
		}

		channel = atoi(argv[2]);
		throttle = atoi(argv[3]);

		if (channel < 1 || channel > 4) {
			printf("Channel should be 1-4.\n");
			return 1;
		}

		return pwm_set_throttle(pwm_lo, channel, throttle);
	} else {
		printf("Unrecognized action `%s'\n", argv[1]);
	}

	return 0;
}