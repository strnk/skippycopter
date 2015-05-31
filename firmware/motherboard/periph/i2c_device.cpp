#include <periph/i2c_device.h>
#include <libopencm3/stm32/i2c.h>

I2C_Device::I2C_Device(uint32_t i2c, uint8_t address) 
	: i2c(i2c), address(address)
{
}


void 
I2C_Device::write(uint8_t reg, size_t size, void* data)
{
	unsigned int i;

	while ((I2C_SR2(i2c) & I2C_SR2_BUSY));

	/*start transfer*/
	i2c_send_start(i2c);
	while (!(I2C_SR1(i2c) & I2C_SR1_SB));

	i2c_send_7bit_address(i2c, address, I2C_WRITE);
	while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));
	while (!(I2C_SR2(i2c) & I2C_SR2_TRA));

	i2c_send_data(i2c, reg);
	while (!(I2C_SR1(i2c) & I2C_SR1_TxE));

	for (i = 0; i < size; i++) {
		i2c_send_data(i2c, ((uint8_t*)data)[i]);
		while (!(I2C_SR1(i2c) & I2C_SR1_TxE));
	}

	i2c_send_stop(i2c);	
}

void 
I2C_Device::write(uint8_t reg, uint8_t value)
{
	uint8_t data = value;
	this->write(reg, 1, &data);
}

void 
I2C_Device::write16(uint8_t reg, uint16_t value)
{
	uint8_t data[2];

	data[0] = value >> 8;
	data[1] = value & 0x0F;

	this->write(reg, 2, &data);
}

void 
I2C_Device::read(uint8_t reg, size_t size, void* data)
{
	uint8_t i;

	while ((I2C_SR2(i2c) & I2C_SR2_BUSY));

	/*start transfer*/
	i2c_send_start(i2c);
	while (!(I2C_SR1(i2c) & I2C_SR1_SB));

	i2c_send_7bit_address(i2c, address, I2C_WRITE);
	while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));
	while (!(I2C_SR2(i2c) & I2C_SR2_TRA));

	i2c_send_data(i2c, reg);
	while (!(I2C_SR1(i2c) & I2C_SR1_TxE));

	/*Setting transfer properties*/
	i2c_send_start(i2c);
	while (!(I2C_SR1(i2c) & I2C_SR1_SB));

	i2c_send_7bit_address(i2c, address, I2C_READ);
	while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));
	while ((I2C_SR2(i2c) & I2C_SR2_TRA));

	if (size == 1) {
		i2c_disable_ack(i2c);
		i2c_send_stop(i2c);
	}
	else
		i2c_enable_ack(i2c);

	for (i = 0; i < size; i++) {
		while (!(I2C_SR1(i2c) & I2C_SR1_RxNE));
		((uint8_t*)data)[i] = i2c_get_data(i2c);

		if (i == size-2) {
			i2c_disable_ack(i2c);
			i2c_send_stop(i2c);	
		}
	}
}

uint8_t 
I2C_Device::read(uint8_t reg)
{
	uint8_t data;
	this->read(reg, 1, &data);

	return data;
}

uint16_t 
I2C_Device::read16(uint8_t reg)
{
	uint8_t data[2];
	this->read(reg, 2, &data);

	return (data[0] << 8) | data[1];
}
