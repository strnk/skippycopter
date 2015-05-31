#ifndef IMU_ADXL345_H
#define IMU_ADXL345_H

#include <imu/imu_device.h>
#include <common.h>

// ADXL345 accelerometer
#define ADXL345_ADDR 				0x53
#define ADXL345_REG_DEVID			0x00
#define ADXL345_REG_THRESH_TAP		0x1D
#define ADXL345_REG_OFSX			0x1E
#define ADXL345_REG_OFSY			0x1F
#define ADXL345_REG_OFSZ			0x20
#define ADXL345_REG_DUR				0x21
#define ADXL345_REG_LATENT			0x22
#define ADXL345_REG_WINDOW			0x23
#define ADXL345_REG_THRESH_ACT		0x24
#define ADXL345_REG_THRESH_INACT	0x25
#define ADXL345_REG_TIME_INACT		0x26
#define ADXL345_REG_ACT_INACT_CTL	0x27
#define ADXL345_REG_THRESH_FF		0x28
#define ADXL345_REG_TIME_FF			0x29
#define ADXL345_REG_TAP_AXES		0x2A
#define ADXL345_REG_ACT_TAP_STATUS	0x2B
#define ADXL345_REG_BW_RATE			0x2C
#define ADXL345_REG_POWER_CTL		0x2D
#define ADXL345_REG_INT_ENABLE		0x2E
#define ADXL345_REG_INT_MAP			0x2F
#define ADXL345_REG_INT_SOURCE		0x30
#define ADXL345_REG_DATA_FORMAT		0x31
#define ADXL345_REG_DATAX			0x32
#define ADXL345_REG_DATAY			0x34
#define ADXL345_REG_DATAZ			0x36
#define ADXL345_REG_FIFO_CTL		0x38
#define ADXL345_REG_FIFO_STATUS		0x39

class ADXL345 : public IMU_Device
{
public:
	ADXL345(uint32_t i2c);

	void init(void);
	bool check(void);

	void read_data(data3* data);
};

#endif // IMU_ADXL345_H