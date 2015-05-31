#ifndef IMU_BMP085_H
#define IMU_BMP085_H

#include <imu/imu_device.h>

// BMP085 pressure sensor
#define BMP085_ADDR 		0x77
#define BMP085_REG_AC1		0xAA
#define BMP085_REG_AC2		0xAC
#define BMP085_REG_AC3		0xAE
#define BMP085_REG_AC4		0xB0
#define BMP085_REG_AC5		0xB2
#define BMP085_REG_AC6		0xB4
#define BMP085_REG_B1		0xB6
#define BMP085_REG_B2		0xB8
#define BMP085_REG_MB		0xBA
#define BMP085_REG_MC		0xBC
#define BMP085_REG_MD		0xBE

class BMP085 : public IMU_Device
{
public:
	BMP085(uint32_t i2c);

	void init(void);
	bool check(void);
};

#endif // IMU_BMP085_H