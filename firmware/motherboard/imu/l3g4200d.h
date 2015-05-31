#ifndef IMU_L3G4200D_H
#define IMU_L3G4200D_H

#include <imu/imu_device.h>
#include <common.h>

// L3G4200D gyroscope
#define L3G4200D_ADDR			0x68
#define L3G4200D_REG_WHO_AM_I	0x0F
#define L3G4200D_REG_CTRL1		0x20
#define L3G4200D_REG_CTRL2		0x21
#define L3G4200D_REG_CTRL3		0x22
#define L3G4200D_REG_CTRL4		0x23
#define L3G4200D_REG_CTRL5		0x24
#define L3G4200D_REG_REFERENCE	0x25
#define L3G4200D_REG_OUT_TEMP	0x26
#define L3G4200D_REG_STATUS		0x27
#define L3G4200D_REG_OUT_X		0x28
#define L3G4200D_REG_OUT_Y		0x2A
#define L3G4200D_REG_OUT_Z		0x2C
#define L3G4200D_REG_FIFO_CTRL	0x2E
#define L3G4200D_REG_FIFO_SRC	0x2F
#define L3G4200D_REG_INT1_CFG	0x30

class L3G4200D : public IMU_Device
{
public:
	uint8_t task;
	bool ready;
	data3 gyro;

	L3G4200D(uint32_t i2c);

	void init(void);
	bool check(void);

	void update(void);
	static void update_tick_handler(uint32_t* data) { ((L3G4200D*)data)->update(); }
};

#endif // IMU_L3G4200D_H