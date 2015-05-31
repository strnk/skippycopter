#include <imu/bmp085.h>

BMP085::BMP085(uint32_t i2c)
	: IMU_Device(i2c, BMP085_ADDR)
{

}

void 
BMP085::init(void)
{
}

bool 
BMP085::check(void)
{
	uint16_t ac1;
	ac1 = read16(BMP085_REG_AC1);

	if (ac1 == 0x0000 || ac1 == 0xFFFF)
		return false;
	return true;
}