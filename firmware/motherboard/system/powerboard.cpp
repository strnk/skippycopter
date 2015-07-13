#include <system/powerboard.h>
#include <periph/sys_scheduler.h>

Powerboard::Powerboard(uint32_t i2c)
	: I2C_Device(i2c, PWB_ADDR)
{
}

void 
Powerboard::init(void)
{
    task = SystemScheduler.registerTask(SYS_SCHEDULER_MS(200), Powerboard::update_tick_handler, (uint32_t*)(this));
}

bool 
Powerboard::check(void)
{
	uint8_t id;
	id = read(PWB_REG_DEVID);

	if (id != 0x42)
		return false;
	return true;
}

void
Powerboard::update(void)
{
	raw_voltage = read16(PWB_REG_READ_VOLTAGE);
	raw_intensity = read16(PWB_REG_READ_INTENSITY);
	raw_voltage_avg = read16(PWB_REG_READ_VOLTAGE_AVG);
	raw_intensity_avg = read16(PWB_REG_READ_INTENSITY_AVG);

	voltage = raw_voltage * 0.027275391;
	intensity = (raw_intensity-511) * 0.02;
	
	intensity_avg = (raw_intensity_avg-511) * 0.02;
	voltage_avg = raw_voltage_avg * 0.027275391;
}