#include <libopencm3/stm32/i2c.h>
#include <inttypes.h>
#include "i2c.h"

void i2c_read_multiple(uint32_t i2c, uint8_t i2c_addr, uint8_t reg, uint8_t size,
	      uint8_t *data)
{
	int i;

	//I2C_CR1(i2c) |= I2C_CR1_NOSTRETCH;
	while ((I2C_SR2(i2c) & I2C_SR2_BUSY));

	/*start transfer*/
	i2c_send_start(i2c);
	while (!(I2C_SR1(i2c) & I2C_SR1_SB));

	i2c_send_7bit_address(i2c, i2c_addr, I2C_WRITE);
	while (!(I2C_SR1(i2c) & I2C_SR1_ADDR));
	while (!(I2C_SR2(i2c) & I2C_SR2_TRA));

	i2c_send_data(i2c, reg);
	while (!(I2C_SR1(i2c) & I2C_SR1_TxE));

	/*Setting transfer properties*/
	i2c_send_start(i2c);
	while (!(I2C_SR1(i2c) & I2C_SR1_SB));

	i2c_send_7bit_address(i2c, i2c_addr, I2C_READ);
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
		data[i] = i2c_get_data(i2c);

		if (i == size-2) {
			i2c_disable_ack(i2c);
			i2c_send_stop(i2c);	
		}
	}
}
