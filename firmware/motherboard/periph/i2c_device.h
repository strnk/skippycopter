#ifndef I2C_DEVICE_H
#define I2C_DEVICE_H

#include <stddef.h>
#include <inttypes.h>


class I2C_Device
{
private:
	uint32_t i2c;
	uint8_t address;

public:
	I2C_Device(uint32_t i2c, uint8_t address);
	virtual ~I2C_Device() { };


	void write(uint8_t reg, size_t size, void* data);
	void write(uint8_t reg, uint8_t value);
	void write16(uint8_t reg, uint16_t value);

	void read(uint8_t reg, size_t size, void* data);
	uint8_t read(uint8_t reg);
	uint16_t read16(uint8_t reg);
};

#endif // I2C_DEVICE_H