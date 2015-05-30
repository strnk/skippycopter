#ifndef I2C_H
#define I2C_H

uint8_t i2c_read(uint32_t i2c, uint8_t i2c_addr, uint8_t reg);
void i2c_read_multiple(uint32_t i2c, uint8_t i2c_addr, uint8_t reg, uint8_t size, 
	uint8_t *data);

void i2c_write(uint32_t i2c, uint8_t i2c_addr, uint8_t reg, uint8_t value);
void i2c_write_multiple(uint32_t i2c, uint8_t i2c_addr, uint8_t reg, uint8_t size, 
	uint8_t *data);

#endif // I2C_H