#include <periph/sys_scheduler.h>
#include <imu/adxl345.h>

ADXL345::ADXL345(uint32_t i2c)
	: IMU_Device(i2c, ADXL345_ADDR), task(0), ready(false), accelero({0, 0, 0})
{
}

void 
ADXL345::init(void)
{
	// Disable tap detection
	write(ADXL345_REG_TAP_AXES, 0x00);

	// Low power 	= 0
	// Rate 		= 1101 => 800Hz
	write(ADXL345_REG_BW_RATE, 0x0C);

	// Link = 0
	// Auto sleep = 0
	// Measure = 1
	// Sleep = 0
	// Wake up = 00
	write(ADXL345_REG_POWER_CTL, 0x08);

	// Disable interrupts
	write(ADXL345_REG_INT_ENABLE, 0x00);

	// Self test = 0
	// SPI = 0 => 3-wires
	// INT invert = 0
	// Full res = 0 => 10 bits
	// Justify = 0
	// Range = 01 => +/- 4g
	// => 4mG/LSB
	write(ADXL345_REG_DATA_FORMAT, 0x01);

	// Bypass mode (no FIFO)
	write(ADXL345_REG_FIFO_CTL, 0x00);

    task = SystemScheduler.registerTask(SYS_SCHEDULER_MS(1), ADXL345::update_tick_handler, (uint32_t*)(this));
}

bool 
ADXL345::check(void)
{
	uint8_t id;
	id = read(ADXL345_REG_DEVID);

	if (id != 0xE5)
		return false;
	return true;
}

void
ADXL345::update(void)
{
	uint8_t raw[6];

	read(ADXL345_REG_DATAX, 6, raw);

	accelero.x = (raw[1]<<8) | raw[0];
	accelero.y = (raw[3]<<8) | raw[2];
	accelero.z = (raw[5]<<8) | raw[4];
	ready = true;
}