#ifndef IMU_HMC5883L_H
#define IMU_HMC5883L_H

#include <imu/imu_device.h>
#include <common.h>

// HMC5883L magnetometer
#define HMC5883L_ADDR 		0x1E
#define HMC5883L_REG_CRA	0X00
#define HMC5883L_REG_CRB	0X01
#define HMC5883L_REG_MR		0X02
#define HMC5883L_REG_DX		0X03
#define HMC5883L_REG_DZ		0X05
#define HMC5883L_REG_DY		0X07
#define HMC5883L_REG_SR		0X09
#define HMC5883L_REG_IDA	0X0A
#define HMC5883L_REG_IDB	0X0B
#define HMC5883L_REG_IDC	0X0C

class HMC5883L : public IMU_Device
{
public:
	uint8_t task;
	bool ready;
	data3 magneto;

	HMC5883L(uint32_t i2c);

	void init(void);
	bool check(void);

	void update(void);
	static void update_tick_handler(uint32_t* data) { ((HMC5883L*)data)->update(); }
};

#endif // IMU_HMC5883L_H