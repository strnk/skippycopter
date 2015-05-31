#include <imu/bmp085.h>
#include <periph/sys_scheduler.h>

BMP085::BMP085(uint32_t i2c)
	: IMU_Device(i2c, BMP085_ADDR), cal({0,0,0,0,0,0,0,0,0,0,0}), state(UT_UNINITIALIZED),
	ut(0), up(0), task(0), ready(false), pressure(0), temperature(0)
{

}

void 
BMP085::init(void)
{
	cal.AC1 = read16(BMP085_REG_AC1);
	cal.AC2 = read16(BMP085_REG_AC2);
	cal.AC3 = read16(BMP085_REG_AC3);
	cal.AC4 = read16(BMP085_REG_AC4);
	cal.AC5 = read16(BMP085_REG_AC5);
	cal.AC6 = read16(BMP085_REG_AC6);
	cal.B1 = read16(BMP085_REG_B1);
	cal.B2 = read16(BMP085_REG_B2);
	cal.MB = read16(BMP085_REG_MB);
	cal.MC = read16(BMP085_REG_MC);
	cal.MD = read16(BMP085_REG_MD);

	state = UT_CTRL_WRITE;
    task = SystemScheduler.registerTask(SYS_SCHEDULER_US(500), BMP085::update_tick_handler, (uint32_t*)(this));
}

bool 
BMP085::check(void)
{
	uint16_t ac1;
	ac1 = read16(BMP085_REG_AC1);

	if (ac1 == 0x0000 || ac1 == 0xFFFF)
		return false;
	return true;
}


uint32_t 
BMP085::read24(uint8_t reg)
{
	uint8_t data[3];
	this->read(reg, 3, &data);

	return (data[0] << 16) | (data[1] << 8) | data[2];
}

void
BMP085::update(void)
{
	static int u500;

	switch (state) {
		case UT_UNINITIALIZED:
		break;

		case UT_CTRL_WRITE:
			// Request temperature 
			write(BMP085_REG_CTRL, 0x2E);

			u500 = 9;
			state = UT_WAIT;
		break;

		case UT_WAIT:
			// Wait for 4.5ms
			u500--;
			if (u500 == 0)
				state = UT_READ;
		break;

		case UT_READ:
			ut = (int16_t)read16(BMP085_REG_DATA);
			state = UP_CTRL_WRITE;
		//break;

		case UP_CTRL_WRITE:
			// Request pressure (over-sampling = 0)
			write(BMP085_REG_CTRL, 0x34 | (BMP085_OVERSAMPLING << 6));
			u500 = 9 * (BMP085_OVERSAMPLING+1);
			state = UP_WAIT;
		break;

		case UP_WAIT:
			// Wait for 4.5ms
			u500--;
			if (u500 == 0)
				state = UP_READ;
		break;

		case UP_READ:
			up = (int32_t)read24(BMP085_REG_DATA) >> (8-BMP085_OVERSAMPLING);
			state = UPDATE;
		//break;

		case UPDATE:
			_compensate();
			ready = true;
			state = UT_CTRL_WRITE;
		break;

		default:
			state = UT_CTRL_WRITE;
	}
}

void 
BMP085::_compensate(void)
{
	int32_t B3, B5, B6, X1, X2, X3;
	uint32_t B4, B7;

	// True temperature
	X1 = ((ut - cal.AC6) * cal.AC5) >> 15;
	X2 = (cal.MC << 11) / (X1 + cal.MD);
	B5 = X1 + X2;
	temperature = (B5 + 8) >> 4;

	// True pressure
	B6 = B5-4000;
	X1 = (cal.B2 * (B6 * (B6 >> 12))) >> 11;
	X2 = cal.AC2 * (B6>>11);
	X3 = X1 + X2;

	B3 = ((((cal.AC1<<2) + X3) << BMP085_OVERSAMPLING) + 2) >> 2;
	X1 = (cal.AC3 * B6) >> 13;
	X2 = (cal.B1 * (B6 * (B6>>12))) >> 16;
	X3 = ((X1 + X2) + 2) >> 2;
	B4 = (cal.AC4 * (uint32_t)(X3 + (1<<15))) >> 15;
	B7 = ((uint32_t)up - B3) * (50000 >> BMP085_OVERSAMPLING);

	if (B7 < 0x80000000)
		pressure = (B7<<1) / B4;
	else
		pressure = (B7/B4) << 1;

	X1 = (pressure>>8) * (pressure>>8);
	X1 = (X1 * 3038) >> 16;
	X2 = (-7357*pressure) >> 16;
	pressure = pressure + ((X1 + X2 + 3791) >> 4);
}