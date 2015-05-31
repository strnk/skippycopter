#include <imu/hmc5883l.h>

HMC5883L::HMC5883L(uint32_t i2c)
	: IMU_Device(i2c, HMC5883L_ADDR)
{

}

void 
HMC5883L::init(void)
{
	// MA1:0 = 11	=> 8 samples averaged
	// D02:0 = 110	=> 75 Hz sample rate
	// MS1:0 = 00	=> No bias
	write(HMC5883L_REG_CRA, 0xF8);

	// GN2:0 = 010	=> 1090 LSB/Gauss
	write(HMC5883L_REG_CRB, 0x20);

	// MD1:0 = 00	=> Continous measurement mode
	write(HMC5883L_REG_MR, 0x00);
}

bool 
HMC5883L::check(void)
{
	uint8_t idA, idB, idC;
	idA = read(HMC5883L_REG_IDA);
	idB = read(HMC5883L_REG_IDB);
	idC = read(HMC5883L_REG_IDC);

	if (idA != 0x48 || idB != 0x34 || idC != 0x33)
		return false;

	return true;
}

void
HMC5883L::read_data(data3* data)
{
	uint8_t raw[6];

	read(HMC5883L_REG_DX, 6, raw);

	data->x = (raw[0]<<8) | raw[1];
	data->z = (raw[2]<<8) | raw[3];
	data->y = (raw[4]<<8) | raw[5];
}