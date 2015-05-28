#ifndef IMU_H
#define IMU_H

#define IMU_HMC5883L_ADDR 		0x1E
#define IMU_HMC5883L_REG_IDA	0X0A
#define IMU_HMC5883L_REG_IDB	0X0B
#define IMU_HMC5883L_REG_IDC	0X0C


#define IMU_ADXL345_ADDR 		0x53
#define IMU_ADXL345_REG_DEVID	0x00


#define IMU_BMP085_ADDR 		0x77
#define IMU_BMP085_REG_AC1		0xAA
#define IMU_BMP085_REG_AC2		0xAC
#define IMU_BMP085_REG_AC3		0xAE
#define IMU_BMP085_REG_AC4		0xB0
#define IMU_BMP085_REG_AC5		0xB2
#define IMU_BMP085_REG_AC6		0xB4
#define IMU_BMP085_REG_B1		0xB6
#define IMU_BMP085_REG_B2		0xB8
#define IMU_BMP085_REG_MB		0xBA
#define IMU_BMP085_REG_MC		0xBC
#define IMU_BMP085_REG_MD		0xBE

#define IMU_L3G4200D_ADDR			0x68
#define IMU_L3G4200D_REG_WHO_AM_I	0x0F
#define IMU_L3G4200D_REG_OUT_TEMP	0x26


void imu_setup(void);
uint8_t hmc5883l_read(uint8_t reg);
uint8_t adxl345_read(uint8_t reg);
uint16_t bmp085_read(uint8_t reg);
uint8_t l3g4200d_read(uint8_t reg);
uint16_t l3g4200d_read16(uint8_t reg);

void read_i2c(uint32_t i2c, uint8_t i2c_addr, uint8_t reg, uint8_t size, uint8_t *data);

#endif