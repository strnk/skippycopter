#ifndef I2C_H
#define I2C_H

void i2c_read_multiple(uint32_t i2c, uint8_t i2c_addr, uint8_t reg, uint8_t size, 
	uint8_t *data);

#endif // I2C_H