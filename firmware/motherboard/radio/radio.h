#ifndef RADIO_RADIO_H
#define RADIO_RADIO_H

#include <inttypes.h>

typedef struct {
	float throttle;
	float yaw;
	float pitch;
	float roll;
	float aux1;
	float aux2;
} radio_cmd_t;

class Radio
{
public:
	radio_cmd_t cmd;

	Radio(void);

	void update_from_ppm(uint32_t ppm_pulses[]);

private:
	inline float _bound(float x, float min, float max) { if (x < min) { return min; } else if (x > max) { return max; } else return x; }
};
#endif // RADIO_RADIO_H