#include <imu/adxl345.h>

ADXL345::ADXL345(uint32_t i2c)
	: IMU_Device(i2c, ADXL345_ADDR)
{

}

void 
ADXL345::init(void)
{
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