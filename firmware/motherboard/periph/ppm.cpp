#include <stdio.h>
#include <periph/ppm.h>
#include <config.h>
#include <radio.h>

#define PPM_TICKS_TO_USEC(x)	(x/6)

static uint32_t ppm_overflow = 0;
PPM* ppmHandler;

PPM::PPM(uint32_t timer, uint8_t channels)
	: timer(timer), channels(channels), pulses({0,}), invalid(false), pending(false), invalid_frames(0), 
		previous_pulse(0), current_pulse(0)
{
}

void
PPM::update(uint32_t timer_value)
{
	uint32_t pulse = timer_value - previous_pulse;
	previous_pulse = timer_value;

	if (current_pulse == 0) {
		if (PPM_TICKS_TO_USEC(pulse) >= PPM_BLANK_MIN && PPM_TICKS_TO_USEC(pulse) <= PPM_BLANK_MAX) {
			pulses[current_pulse++] = pulse;
			invalid = false;
		}
	} else {
		if (PPM_TICKS_TO_USEC(pulse) >= PPM_DATA_MIN && PPM_TICKS_TO_USEC(pulse) <= PPM_DATA_MAX) {
			pulses[current_pulse++] = pulse;
		} else {
			invalid = true;
			invalid_frames++;
			current_pulse = 0;
		}
	}

	if (current_pulse > channels) {
		current_pulse = 0;
		pending = true;
	}
}

void
PPM::init(enum tim_ic_id channel, enum tim_ic_input input)
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

	// TCLK = 168MHz => timer increment @ 1/6th us
	timer_set_prescaler(timer, 28);
	timer_set_period(timer, 0xFFFF);
	timer_ic_set_polarity(timer, channel, TIM_IC_RISING);
	timer_ic_set_input(timer, channel, input);
	timer_ic_set_prescaler(timer, channel, TIM_IC_PSC_OFF);
	timer_ic_set_filter(timer, channel, TIM_IC_OFF);

	// Link to the PPM handler
	ppmHandler = this;

	nvic_enable_irq(IRQ_PPM);
#ifdef IRQ_PPM_ALT
	nvic_enable_irq(IRQ_PPM_ALT);
#endif

	// Enable capture/compare and update interrupts
	timer_enable_irq(timer, (PPM_TIMER_CC_IE | TIM_DIER_UIE));

	// Enable PPM IC channel and timer
	timer_ic_enable(timer, channel);
	timer_enable_counter(timer);
}

void tim_ppm_isr(void) {
	if ((PPM_TIMER_SR & PPM_TIMER_CC_IF) != 0) {
		timer_clear_flag(PPM_TIMER, PPM_TIMER_CC_IF);
		uint32_t now = timer_get_counter(PPM_TIMER) | ppm_overflow;
		ppmHandler->update(now);
	}
}

void tim_ppm_alt_isr(void) {
	if ((PPM_TIMER_SR & TIM_SR_UIF) != 0) {
		ppm_overflow += (1 << 16);
		timer_clear_flag(PPM_TIMER, TIM_SR_UIF);
	}
}