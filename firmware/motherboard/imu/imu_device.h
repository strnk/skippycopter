#ifndef IMU_DEVICE_H
#define IMU_DEVICE_H

#include <periph/i2c_device.h>

class IMU_Device : public I2C_Device
{
public:
	IMU_Device(uint32_t i2c, uint8_t addr) : I2C_Device(i2c, addr) { };
	virtual ~IMU_Device() { };

	virtual void init(void) = 0;
	virtual bool check(void) = 0;
};

#endif // IMU_DEVICE_H