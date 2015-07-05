#include <periph/pwm.h>

PWM::PWM(uint32_t timer, uint32_t min, uint32_t max)
	: timer(timer), min(min), max(max)
{
	throttle_coeff = (float)(max-min)/1000.0;
}

void
PWM::init(uint32_t prescaler, uint32_t period)
{
	/* Reset the timer peripheral */
	timer_reset(timer);

	/* Set the timers global mode to:
	* - use no divider
	* - alignment edge
	* - count direction up
	*/
	timer_set_mode(timer, 
		TIM_CR1_CKD_CK_INT,
		TIM_CR1_CMS_EDGE,
		TIM_CR1_DIR_UP);

	timer_set_prescaler(timer, prescaler);
	timer_set_repetition_counter(timer, 0);
	timer_enable_preload(timer);
	timer_continuous_mode(timer);
	timer_set_period(timer, period);
	timer_enable_counter(timer);
}

void
PWM::arm(tim_oc_id channel)
{
	timer_set_oc_mode(timer, channel, TIM_OCM_PWM1);
	timer_set_oc_value(timer, channel, 0);
	timer_enable_oc_output(timer, channel);
	timer_generate_event(timer, TIM_EGR_UG);
}

void
PWM::disable(tim_oc_id channel)
{
	timer_disable_oc_output(timer, channel);
}

void
PWM::stop(tim_oc_id channel)
{
	set_pulse_width(channel, min);
}

// Throttle is in thousands (from 0 to 1000)
void
PWM::set_throttle(tim_oc_id channel, uint16_t throttle)
{
	set_pulse_width(channel, (uint32_t)(throttle * throttle_coeff) + min);
}

void
PWM::set_pulse_width(tim_oc_id channel, uint32_t pulse)
{
	timer_set_oc_value(timer, channel, pulse);
}
