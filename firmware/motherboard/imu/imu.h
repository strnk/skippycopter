#ifndef IMU_IMU_H
#define IMU_IMU_H

#include <libopencm3/stm32/rcc.h>
#include <imu/adxl345.h>
#include <imu/bmp085.h>
#include <imu/hmc5883l.h>
#include <imu/l3g4200d.h>
#include <imu/fusion/madgwick.h>

// imu_check() failure codes
#define IMU_CHECK_OK				0x00
#define IMU_CHECK_HMC5883L_FAIL		0x01
#define IMU_CHECK_ADXL345_FAIL		0x02
#define IMU_CHECK_BMP085_FAIL		0x04
#define IMU_CHECK_L3G4200D_FAIL		0x08

#define IMU_ACCELEROMETER			0x01
#define IMU_GYROMETER				0x02
#define IMU_MAGNETOMETER			0x04
#define IMU_BAROMETER				0x08

typedef uint8_t imu_check_t;

class IMU
{
private:
	uint8_t task;
	uint32_t i2c;
	rcc_periph_clken i2c_rcc;
	MadgwickAHRS fusion;

public:
	ADXL345 adxl345;
	BMP085 bmp085;
	HMC5883L hmc5883l;
	L3G4200D l3g4200d;

	data3f gyro;
	data3f accelero;
	data3f magneto;
	float temperature;
	float pressure;

public:
	IMU(uint32_t i2c, rcc_periph_clken i2c_rcc)
		: task(0), i2c(i2c), i2c_rcc(i2c_rcc), fusion(), adxl345(i2c), bmp085(i2c), hmc5883l(i2c), l3g4200d(i2c),
		gyro({.0,.0,.0}), accelero({.0,.0,.0}), magneto({.0,.0,.0}), temperature(.0), pressure(.0)
		{ }

	void setup();
	void init(void);
	imu_check_t check(void);

	void checkPending(void);
	void update(uint8_t what);
	void fusionUpdate(void);

	static void update_tick_handler(uint32_t* data) { ((IMU*)data)->fusionUpdate(); }

	orientation_t getOrientation() { return fusion.getOrientation(); }
	quaternion_t getQuaternion() { return fusion.getQuaternion(); }
};
#endif // IMU_IMU_H