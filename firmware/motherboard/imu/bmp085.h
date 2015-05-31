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
#define BMP085_REG_CTRL		0xF4
#define BMP085_REG_DATA		0xF6

#define BMP085_OVERSAMPLING	0

typedef struct 
{
	int16_t AC1;
	int16_t AC2;
	int16_t AC3;
	uint16_t AC4;
	uint16_t AC5;
	uint16_t AC6;
	int16_t B1;
	int16_t B2;
	int16_t MB;
	int16_t MC;
	int16_t MD;
} bmp085_calibration_data_t;

typedef enum
{
	UT_UNINITIALIZED = 0,
	UT_CTRL_WRITE,
	UT_WAIT,
	UT_READ,
	UP_CTRL_WRITE,
	UP_WAIT,
	UP_READ,
	UPDATE
} bmp085_state_t;

class BMP085 : public IMU_Device
{
private:
	bmp085_calibration_data_t cal;
	bmp085_state_t state;
	int32_t ut, up;

public:
	uint8_t task;
	bool ready;
	int32_t pressure;
	int32_t temperature;

	BMP085(uint32_t i2c);

	void init(void);
	bool check(void);

	uint32_t read24(uint8_t reg);

	void update(void);
	static void update_tick_handler(uint32_t* data) { ((BMP085*)data)->update(); }

private:
	void _compensate(void);
};

#endif // IMU_BMP085_H