#include <periph/sys_scheduler.h>
#include <imu/l3g4200d.h>

L3G4200D::L3G4200D(uint32_t i2c)
	: IMU_Device(i2c, L3G4200D_ADDR), task(0), ready(false), gyro({0, 0, 0})
{

}

void 
L3G4200D::init(void)
{
	// 800Hz, LPF 110Hz, X, Y and Z enabled
	write(L3G4200D_REG_CTRL1, 0xFF);

	// HPF normal mode, 0.1Hz
	write(L3G4200D_REG_CTRL2, 0x09);

	// Disable interrupts
	write(L3G4200D_REG_CTRL3, 0x00);

	// Continuous update, big endian, full scale 2000dps
	write(L3G4200D_REG_CTRL4, 0xB0);

	// No FIFO, output not HPF
	write(L3G4200D_REG_CTRL5, 0x00);

	// Bypass mode
	write(L3G4200D_REG_FIFO_CTRL, 0x00);

    task = SystemScheduler.registerTask(SYS_SCHEDULER_MS(1), L3G4200D::update_tick_handler, (uint32_t*)(this));
}

bool 
L3G4200D::check(void)
{
	uint8_t id;
	id = read(L3G4200D_REG_WHO_AM_I);

	if (id != 0xD3)
		return false;
	return true;
}

void
L3G4200D::update(void)
{
	uint8_t raw[6];
	read(L3G4200D_REG_OUT_X | 0x80, 6, raw);

	gyro.x = (raw[1]<<8) | raw[0];
	gyro.y = (raw[3]<<8) | raw[2];
	gyro.z = (raw[5]<<8) | raw[4];
	ready = true;
}