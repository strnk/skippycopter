#ifndef IMU_IMU_H
#define IMU_IMU_H

#include <libopencm3/stm32/rcc.h>
#include <imu/adxl345.h>
#include <imu/bmp085.h>
#include <imu/hmc5883l.h>
#include <imu/l3g4200d.h>

// imu_check() failure codes
#define IMU_CHECK_OK				0x00
#define IMU_CHECK_HMC5883L_FAIL		0x01
#define IMU_CHECK_ADXL345_FAIL		0x02
#define IMU_CHECK_BMP085_FAIL		0x04
#define IMU_CHECK_L3G4200D_FAIL		0x08

typedef uint8_t imu_check_t;

class IMU
{
private:
	uint32_t i2c;
	rcc_periph_clken i2c_rcc;

public:
	ADXL345 adxl345;
	BMP085 bmp085;
	HMC5883L hmc5883l;
	L3G4200D l3g4200d;

public:
	IMU(uint32_t i2c, rcc_periph_clken i2c_rcc)
		: i2c(i2c), i2c_rcc(i2c_rcc), adxl345(i2c), bmp085(i2c), hmc5883l(i2c), l3g4200d(i2c) 
		{ }

	void setup();
	void init(void);
	imu_check_t check(void);
};
#endif // IMU_IMU_H