#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <inttypes.h>
#include "imu.h"
#include "i2c.h"
#include "config.h"

void 
imu_setup(void)
{
	rcc_periph_clock_enable(RCC_I2C_SENSOR);

	i2c_reset(I2C_SENSOR);
	i2c_peripheral_disable(I2C_SENSOR);

	i2c_set_clock_frequency(I2C_SENSOR, rcc_apb1_frequency/1000000);
	i2c_set_fast_mode(I2C_SENSOR);
	i2c_set_dutycycle(I2C_SENSOR, I2C_CCR_DUTY_16_DIV_9);

	// For APB1 PCLK1 = 42MHz => I2C speed = 400kHz
	i2c_set_ccr(I2C_SENSOR, 3);

	i2c_peripheral_enable(I2C_SENSOR);
}

uint8_t
hmc5883l_read(uint8_t reg)
{
	uint8_t val;

	i2c_read_multiple(I2C_SENSOR, IMU_HMC5883L_ADDR, reg, 1, &val);
	return val;
}


uint8_t
adxl345_read(uint8_t reg)
{
	uint8_t val;

	i2c_read_multiple(I2C_SENSOR, IMU_ADXL345_ADDR, reg, 1, &val);
	return val;
}

uint16_t
bmp085_read(uint8_t reg)
{
	uint8_t val[2];

	i2c_read_multiple(I2C_SENSOR, IMU_BMP085_ADDR, reg, 2, &val);
	return val[0]<<8 | val[1];
}

uint8_t l3g4200d_read(uint8_t reg)
{
	uint8_t val;

	i2c_read_multiple(I2C_SENSOR, IMU_L3G4200D_ADDR, reg, 1, &val);
	return val;
}

uint16_t l3g4200d_read16(uint8_t reg)
{
	uint8_t val[2];

	i2c_read_multiple(I2C_SENSOR, IMU_L3G4200D_ADDR, reg, 2, &val);
	return val[0]<<8 | val[1];	
}
