#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/gpio.h>
#include <periph/sys_scheduler.h>
#include <debug/performance.h>
#include <imu/imu.h>
#include <stdio.h>
#include <config.h>

void
IMU::setup()
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
IMU::init(void)
{
	adxl345.init();
	bmp085.init();
	hmc5883l.init();
	l3g4200d.init();

    task = SystemScheduler.registerTask(SYS_SCHEDULER_MS(2), IMU::update_tick_handler, (uint32_t*)(this));
}

void
IMU::fusionUpdate(void)
{
	fusion.update(gyro, accelero, magneto);
}

imu_check_t
IMU::check(void)
{
	imu_check_t check = IMU_CHECK_OK;

    gpio_mode_setup(GPIOB, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO6 | GPIO7);
    if (!gpio_get(GPIOB, GPIO6) || !gpio_get(GPIOB, GPIO7)) {
    	printf("I2CERR");
    }
    gpio_mode_setup(GPIOB, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7);

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

void
IMU::checkPending(void)
{
	uint8_t pending = 0;

	if (hmc5883l.ready) {
		hmc5883l.ready = false;
		pending |= IMU_MAGNETOMETER;
		perf_counter_inc(PERF_IMU_MAGN);
	}

	if (adxl345.ready) {
		adxl345.ready = false;
		pending |= IMU_ACCELEROMETER;
		perf_counter_inc(PERF_IMU_ACCEL);
	}

	if (l3g4200d.ready) {
		l3g4200d.ready = false;
		pending |= IMU_GYROMETER;
		perf_counter_inc(PERF_IMU_GYRO);
	}

	if (bmp085.ready) {
		bmp085.ready = false;
		pending |= IMU_BAROMETER;
		perf_counter_inc(PERF_IMU_PRESS);
	}

	update(pending);
}


void 
IMU::update(uint8_t what)
{
	if (what & IMU_ACCELEROMETER) {
		accelero.x = adxl345.accelero.x * 0.008;
		accelero.y = adxl345.accelero.y * 0.008;
		accelero.z = adxl345.accelero.z * 0.008;
	}

	if (what & IMU_GYROMETER) {
		gyro.x = l3g4200d.gyro.x * 0.07 * (6.283185/360);
		gyro.y = l3g4200d.gyro.y * 0.07 * (6.283185/360);
		gyro.z = l3g4200d.gyro.z * 0.07 * (6.283185/360);
	}

	if (what & IMU_MAGNETOMETER) {
		magneto.x = hmc5883l.magneto.x * 0.92;
		magneto.y = hmc5883l.magneto.y * 0.92;
		magneto.z = hmc5883l.magneto.z * 0.92;
	}

	if (what & IMU_BAROMETER) {
		temperature = bmp085.temperature / 10;
		pressure = bmp085.pressure / 100;
	}
}