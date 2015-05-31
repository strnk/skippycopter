#ifndef IMU_L3G4200D_H
#define IMU_L3G4200D_H

#include <imu/imu_device.h>

// L3G4200D gyroscope
#define L3G4200D_ADDR			0x68
#define L3G4200D_REG_WHO_AM_I	0x0F
#define L3G4200D_REG_OUT_TEMP	0x26

class L3G4200D : public IMU_Device
{
public:
	L3G4200D(uint32_t i2c);

	void init(void);
	bool check(void);
};

#endif // IMU_L3G4200D_H