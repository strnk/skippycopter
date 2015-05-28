#include <stdio.h>
#include "board_setup.h"
#include "debug.h"
#include "led.h"
#include "imu.h"
#include "config.h"

int main(void)
{
	int i;

	clock_setup();
	gpio_setup();
	debug_setup();
    imu_setup();

	/* Set two LEDs for wigwag effect when toggling. */
	led_set(LED1);
	
	printf("Skippycopter v1.0\n");


    {
        uint8_t id;
        id = adxl345_read(IMU_ADXL345_REG_DEVID);

        printf("ADXL345 ID reg: %x\n", id); 
    }

    {
        uint8_t idA, idB, idC;
        idA = hmc5883l_read(IMU_HMC5883L_REG_IDA);
        idB = hmc5883l_read(IMU_HMC5883L_REG_IDB);
        idC = hmc5883l_read(IMU_HMC5883L_REG_IDC);

        printf("HMC5883L ID regs: %x %x %x\n", idA, idB, idC);
    }

    {
        uint16_t ac1;
        ac1 = bmp085_read(IMU_BMP085_REG_AC1);

        printf("BMP085 AC1 reg: %x\n", ac1); 
    }

    {
        uint8_t id, temperature;
        id = l3g4200d_read(IMU_L3G4200D_REG_WHO_AM_I);
        temperature = l3g4200d_read(IMU_L3G4200D_REG_OUT_TEMP);

        printf("L3G4200D WHOAMI reg: %x\n", id); 
        printf("L3G4200D OUT_TEMP reg: %x\n", temperature); 
    }

	/* Blink the LEDs (PD12, PD13, PD14 and PD15) on the board. */
	while (1) {
		/* Toggle LEDs. */
		led_toggle(LED1 | LED2);
		//printf(".\n");
		for (i = 0; i < 6000000; i++) { /* Wait a bit. */
			__asm__("nop");
		}
	}

	return 0;
}
