#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/i2c.h>
#include <inttypes.h>
#include "imu.h"
#include "i2c.h"
#include "config.h"

static void hmc5883l_init(void);


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

void
imu_init(void)
{
	hmc5883l_init();
}

imu_check_t
imu_check(void)
{
	imu_check_t check = IMU_CHECK_OK;

	// Check the ADXL345 ID value, should be 0xE5
	{
        uint8_t id;
        id = adxl345_read(ADXL345_REG_DEVID);

        if (id != 0xE5)
        	check |= IMU_CHECK_ADXL345_FAIL;
    }

    // Check the HMC5883L ID A, B and C, should be 0x48, 0x34, 0x33
    {
        uint8_t idA, idB, idC;
        idA = hmc5883l_read(HMC5883L_REG_IDA);
        idB = hmc5883l_read(HMC5883L_REG_IDB);
        idC = hmc5883l_read(HMC5883L_REG_IDC);

        if (idA != 0x48 || idB != 0x34 || idC != 0x33)
        	check |= IMU_CHECK_HMC5883L_FAIL;
    }

    // Check the BMP085 AC1, should not be 0x0 nor 0xFFFF
    {
        uint16_t ac1;
        ac1 = bmp085_read(BMP085_REG_AC1);

        if (ac1 == 0x0000 || ac1 == 0xFFFF)
        	check |= IMU_CHECK_BMP085_FAIL;
    }

    // Check the L3G4200D WHOAMI value, should be 0xD3
    {
        uint8_t id;
        id = l3g4200d_read(L3G4200D_REG_WHO_AM_I);

        if (id != 0xD3)
        	check |= IMU_CHECK_L3G4200D_FAIL;
    }

    return check;
}


static void
hmc5883l_init(void)
{
	// MA1:0 = 11	=> 8 samples averaged
	// D02:0 = 110	=> 75 Hz sample rate
	// MS1:0 = 00	=> No bias
	hmc5883l_write(HMC5883L_REG_CRA, 0xF8);

	// GN2:0 = 010	=> 1090 LSB/Gauss
	hmc5883l_write(HMC5883L_REG_CRB, 0x20);

	// MD1:0 = 00	=> Continous measurement mode
	hmc5883l_write(HMC5883L_REG_MR, 0x00);
}

data3
hmc5883l_read_data(data3* data)
{
	uint8_t raw[6];

	i2c_read_multiple(I2C_SENSOR, HMC5883L_ADDR, HMC5883L_REG_DX, 6, raw);

	data->x = (raw[0]<<8) | raw[1];
	data->z = (raw[2]<<8) | raw[3];
	data->y = (raw[4]<<8) | raw[5];
}

uint8_t
hmc5883l_read(uint8_t reg)
{
	return i2c_read(I2C_SENSOR, HMC5883L_ADDR, reg);
}

void
hmc5883l_write(uint8_t reg, uint8_t value)
{
	i2c_write(I2C_SENSOR, HMC5883L_ADDR, reg, value);
}

uint8_t
adxl345_read(uint8_t reg)
{
	return i2c_read(I2C_SENSOR, ADXL345_ADDR, reg);
}

void
adxl345_write(uint8_t reg, uint8_t value)
{
	i2c_write(I2C_SENSOR, ADXL345_ADDR, reg, value);
}

uint16_t
bmp085_read(uint8_t reg)
{
	uint8_t val[2];

	i2c_read_multiple(I2C_SENSOR, BMP085_ADDR, reg, 2, val);
	return val[0]<<8 | val[1];
}

void
bmp085_write(uint8_t reg, uint16_t value)
{
	uint8_t values[2];
	values[0] = value >> 8;
	values[1] = value & 0xFF;

	i2c_write_multiple(I2C_SENSOR, BMP085_ADDR, reg, 2, values);
}

uint8_t l3g4200d_read(uint8_t reg)
{
	return i2c_read(I2C_SENSOR, L3G4200D_ADDR, reg);
}

uint16_t l3g4200d_read16(uint8_t reg)
{
	uint8_t val[2];

	i2c_read_multiple(I2C_SENSOR, L3G4200D_ADDR, reg, 2, val);
	return val[0]<<8 | val[1];	
}

void
l3g4200d_write(uint8_t reg, uint8_t value)
{
	i2c_write(I2C_SENSOR, L3G4200D_ADDR, reg, value);
}

void
l3g4200d_write16(uint8_t reg, uint16_t value)
{
	uint8_t values[2];
	values[0] = value >> 8;
	values[1] = value & 0xFF;

	i2c_write_multiple(I2C_SENSOR, L3G4200D_ADDR, reg, 2, values);
}
