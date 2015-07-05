#ifndef PWM_H
#define PWM_H

#include <stddef.h>
#include <inttypes.h>

#include <libopencm3/stm32/timer.h>


class PWM
{
protected:
	uint32_t timer;
	uint32_t min;
	uint32_t max;
	float throttle_coeff;

public:
	PWM(uint32_t timer, uint32_t min, uint32_t max);

	void init(uint32_t prescaler, uint32_t period);

	void arm(tim_oc_id channel);
	void disable(tim_oc_id channel);

	void stop(tim_oc_id channel);
	void set_throttle(tim_oc_id channel, uint16_t throttle);
	void set_pulse_width(tim_oc_id channel, uint32_t pulse);
};

#endif // PWM_H