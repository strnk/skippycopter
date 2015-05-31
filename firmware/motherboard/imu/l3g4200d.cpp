#include <imu/l3g4200d.h>

L3G4200D::L3G4200D(uint32_t i2c)
	: IMU_Device(i2c, L3G4200D_ADDR)
{

}

void 
L3G4200D::init(void)
{
}

bool 
L3G4200D::check(void)
{
	uint8_t id;
	id = read(L3G4200D_REG_WHO_AM_I);

	if (id != 0xD3)
		return false;
	return true;
}