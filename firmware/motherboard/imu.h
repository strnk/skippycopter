#ifndef IMU_H
#define IMU_H

#include <inttypes.h>
#include "common.h"

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

// ADXL345 accelerometer
#define ADXL345_ADDR 				0x53
#define ADXL345_REG_DEVID			0x00
#define ADXL345_REG_THRESH_TAP		0x1D
#define ADXL345_REG_OFSX			0x1E
#define ADXL345_REG_OFSY			0x1F
#define ADXL345_REG_OFSZ			0x20
#define ADXL345_REG_DUR				0x21
#define ADXL345_REG_LATENT			0x22
#define ADXL345_REG_WINDOW			0x23
#define ADXL345_REG_THRESH_ACT		0x24
#define ADXL345_REG_THRESH_INACT	0x25
#define ADXL345_REG_TIME_INACT		0x26
#define ADXL345_REG_ACT_INACT_CTL	0x27
#define ADXL345_REG_THRESH_FF		0x28
#define ADXL345_REG_TIME_FF			0x29
#define ADXL345_REG_TAP_AXES		0x2A
#define ADXL345_REG_ACT_TAP_STATUS	0x2B
#define ADXL345_REG_BW_RATE			0x2C
#define ADXL345_REG_POWER_CTL		0x2D
#define ADXL345_REG_INT_ENABLE		0x2E
#define ADXL345_REG_INT_MAP			0x2F
#define ADXL345_REG_INT_SOURCE		0x30
#define ADXL345_REG_DATA_FORMAT		0x31
#define ADXL345_REG_DATAX			0x32
#define ADXL345_REG_DATAY			0x34
#define ADXL345_REG_DATAZ			0x36
#define ADXL345_REG_FIFO_CTL		0x38
#define ADXL345_REG_FIFO_STATUS		0x39

// BMP085 pressure sensor
#define BMP085_ADDR 		0x77
#define BMP085_REG_AC1		0xAA
#define BMP085_REG_AC2		0xAC
#define BMP085_REG_AC3		0xAE
#define BMP085_REG_AC4		0xB0
#define BMP085_REG_AC5		0xB2
#define BMP085_REG_AC6		0xB4
#define BMP085_REG_B1		0xB6
#define BMP085_REG_B2		0xB8
#define BMP085_REG_MB		0xBA
#define BMP085_REG_MC		0xBC
#define BMP085_REG_MD		0xBE

// L3G4200D gyroscope
#define L3G4200D_ADDR			0x68
#define L3G4200D_REG_WHO_AM_I	0x0F
#define L3G4200D_REG_OUT_TEMP	0x26

// imu_check() failure codes
#define IMU_CHECK_OK				0x00
#define IMU_CHECK_HMC5883L_FAIL		0x01
#define IMU_CHECK_ADXL345_FAIL		0x02
#define IMU_CHECK_BMP085_FAIL		0x04
#define IMU_CHECK_L3G4200D_FAIL		0x08

typedef uint8_t imu_check_t;


void imu_setup(void);
imu_check_t imu_check(void);
void imu_init(void);

data3 hmc5883l_read_data(data3* data);

uint8_t hmc5883l_read(uint8_t reg);
uint8_t adxl345_read(uint8_t reg);
uint16_t bmp085_read(uint8_t reg);
uint8_t l3g4200d_read(uint8_t reg);
uint16_t l3g4200d_read16(uint8_t reg);

void hmc5883l_write(uint8_t reg, uint8_t value);
void adxl345_write(uint8_t reg, uint8_t value);
void bmp085_write(uint8_t reg, uint16_t value);
void l3g4200d_write(uint8_t reg, uint8_t value);
void l3g4200d_write16(uint8_t reg, uint16_t value);

#endif