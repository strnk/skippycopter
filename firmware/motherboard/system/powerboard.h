#ifndef SYS_POWERBOARD_H
#define SYS_POWERBOARD_H

#include <periph/i2c_device.h>

// Powerboard
#define PWB_ADDR 					0x42
#define PWB_REG_LED_CTRL			0x00
#define PWB_REG_READ_VOLTAGE		0x01
#define PWB_REG_READ_INTENSITY		0x02
#define PWB_REG_READ_VOLTAGE_AVG	0x03
#define PWB_REG_READ_INTENSITY_AVG	0x04
#define PWB_REG_DEVID				0x80


class Powerboard : public I2C_Device
{
private:
	uint8_t task;

public:
	uint16_t raw_intensity;
	uint16_t raw_voltage;
	uint16_t raw_intensity_avg;
	uint16_t raw_voltage_avg;

	float intensity;
	float voltage;
	float intensity_avg;
	float voltage_avg;


	Powerboard(uint32_t i2c);

	void init(void);
	bool check(void);

	void update(void);
	static void update_tick_handler(uint32_t* data) { ((Powerboard*)data)->update(); }
};

#endif // SYS_POWERBOARD_H