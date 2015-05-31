#include <libopencm3/stm32/i2c.h>
#include <imu/imu.h>

void
IMU::setup(void)
{
	rcc_periph_clock_enable(i2c_rcc);

	i2c_reset(i2c);
	i2c_peripheral_disable(i2c);

	i2c_set_clock_frequency(i2c, rcc_apb1_frequency/1000000);
	i2c_set_fast_mode(i2c);
	i2c_set_dutycycle(i2c, I2C_CCR_DUTY_16_DIV_9);

	// For APB1 PCLK1 = 42MHz => I2C speed = 400kHz
	i2c_set_ccr(i2c, 15);

	i2c_peripheral_enable(i2c);
}

void
IMU::init(void)
{
	adxl345.init();
	bmp085.init();
	hmc5883l.init();
	l3g4200d.init();
}

imu_check_t
IMU::check(void)
{
	imu_check_t check = IMU_CHECK_OK;

	if (!adxl345.check())
		check |= IMU_CHECK_ADXL345_FAIL;

	if (!hmc5883l.check())
		check |= IMU_CHECK_HMC5883L_FAIL;

	if (!bmp085.check())
		check |= IMU_CHECK_BMP085_FAIL;

	if (!l3g4200d.check())
		check |= IMU_CHECK_L3G4200D_FAIL;

	return check;
}