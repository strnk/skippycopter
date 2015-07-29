#include <radio/radio.h>
#include <periph/ppm.h>
#include <radio_config.h>


Radio::Radio()
	: cmd({0.0, })
{

}


void
Radio::update_from_ppm(uint32_t ppm_pulses[])
{
	int32_t tmp;

	tmp = (int32_t)ppm_pulses[RADIO_THR] - PPM_USEC_TO_TICKS(RADIO_THR_NEUTRAL);
	tmp *= (tmp >= 0 ? RADIO_CMD_MAX / (float)PPM_USEC_TO_TICKS(RADIO_THR_MAX - RADIO_THR_NEUTRAL)
								: RADIO_CMD_MIN / (float)PPM_USEC_TO_TICKS(RADIO_THR_MIN - RADIO_THR_NEUTRAL)) ;
	cmd.throttle = _bound(tmp, .0, RADIO_CMD_MAX);

	tmp = (int32_t)ppm_pulses[RADIO_YAW] - PPM_USEC_TO_TICKS(RADIO_YAW_NEUTRAL);
	tmp *= (tmp >= 0 ? RADIO_CMD_MAX / (float)PPM_USEC_TO_TICKS(RADIO_YAW_MAX - RADIO_YAW_NEUTRAL)
								: RADIO_CMD_MIN / (float)PPM_USEC_TO_TICKS(RADIO_YAW_MIN - RADIO_YAW_NEUTRAL)) ;
	cmd.yaw = _bound(tmp, RADIO_CMD_MIN, RADIO_CMD_MAX);

	tmp = (int32_t)ppm_pulses[RADIO_PITCH] - PPM_USEC_TO_TICKS(RADIO_PITCH_NEUTRAL);
	tmp *= (tmp >= 0 ? RADIO_CMD_MAX / (float)PPM_USEC_TO_TICKS(RADIO_PITCH_MAX - RADIO_PITCH_NEUTRAL)
								: RADIO_CMD_MIN / (float)PPM_USEC_TO_TICKS(RADIO_PITCH_MIN - RADIO_PITCH_NEUTRAL)) ;
	cmd.pitch = _bound(tmp, RADIO_CMD_MIN, RADIO_CMD_MAX);

	tmp = (int32_t)ppm_pulses[RADIO_ROLL] - PPM_USEC_TO_TICKS(RADIO_ROLL_NEUTRAL);
	tmp *= (tmp >= 0 ? RADIO_CMD_MAX / (float)PPM_USEC_TO_TICKS(RADIO_ROLL_MAX - RADIO_ROLL_NEUTRAL)
								: RADIO_CMD_MIN / (float)PPM_USEC_TO_TICKS(RADIO_ROLL_MIN - RADIO_ROLL_NEUTRAL)) ;
	cmd.roll = _bound(tmp, RADIO_CMD_MIN, RADIO_CMD_MAX);

	tmp = (int32_t)ppm_pulses[RADIO_AUX1] - PPM_USEC_TO_TICKS(RADIO_AUX1_NEUTRAL);
	tmp *= (tmp >= 0 ? RADIO_CMD_MAX / (float)PPM_USEC_TO_TICKS(RADIO_AUX1_MAX - RADIO_AUX1_NEUTRAL)
								: RADIO_CMD_MIN / (float)PPM_USEC_TO_TICKS(RADIO_AUX1_MIN - RADIO_AUX1_NEUTRAL)) ;
	cmd.aux1 = _bound(tmp, RADIO_CMD_MIN, RADIO_CMD_MAX);

	tmp = (int32_t)ppm_pulses[RADIO_AUX2] - PPM_USEC_TO_TICKS(RADIO_AUX2_NEUTRAL);
	tmp *= (tmp >= 0 ? RADIO_CMD_MAX / (float)PPM_USEC_TO_TICKS(RADIO_AUX2_MAX - RADIO_AUX2_NEUTRAL)
								: RADIO_CMD_MIN / (float)PPM_USEC_TO_TICKS(RADIO_AUX2_MIN - RADIO_AUX2_NEUTRAL)) ;
	cmd.aux2 = _bound(tmp, RADIO_CMD_MIN, RADIO_CMD_MAX);
}