#ifndef PPM_H
#define PPM_H

#include <stddef.h>
#include <inttypes.h>

#include <libopencm3/stm32/timer.h>

#define PPM_MAX_CHANNELS	10


class PPM
{
protected:
	uint32_t timer;

public:
	uint8_t channels;
	volatile uint32_t pulses[PPM_MAX_CHANNELS];
	bool invalid;
	bool pending;
	uint32_t invalid_frames;

private:
	volatile uint32_t previous_pulse;
	volatile uint8_t current_pulse;

public:
	PPM(uint32_t timer, uint8_t channels);

	void init(enum tim_ic_id channel, enum tim_ic_input input);
	void update(uint32_t timer);
};

#endif // PPM_H