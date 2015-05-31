#include <imu/adxl345.h>
#include <stdio.h>

ADXL345::ADXL345(uint32_t i2c)
	: IMU_Device(i2c, ADXL345_ADDR)
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
ADXL345::read_data(data3* data)
{
	uint8_t raw[6];

	read(ADXL345_REG_DATAX, 6, raw);

	data->x = (raw[1]<<8) | raw[0];
	data->z = (raw[3]<<8) | raw[2];
	data->y = (raw[5]<<8) | raw[4];
}