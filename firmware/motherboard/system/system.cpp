#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include <system/system.h>
#include <stdio.h>

void
System::setup(void)
{
	rcc_periph_clock_enable(i2c_rcc);

	i2c_reset(i2c);
	i2c_peripheral_disable(i2c);

	i2c_set_clock_frequency(i2c, rcc_apb1_frequency/1000000);
	i2c_set_fast_mode(i2c);
	i2c_set_dutycycle(i2c, I2C_CCR_DUTY_16_DIV_9);

	// For APB1 PCLK1 = 42MHz => I2C speed = 400kHz
	i2c_set_ccr(i2c, 3);

	i2c_peripheral_enable(i2c);
}

void
System::init(void)
{
	powerboard.init();
}

sys_check_t
System::check(void)
{
	sys_check_t check = SYS_CHECK_OK;

	if (!powerboard.check())
		check |= SYS_CHECK_POWERBOARD_FAIL;

	return check;
}