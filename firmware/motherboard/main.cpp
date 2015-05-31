#include <stdio.h>
#include <imu/imu.h>
#include "board_setup.h"
#include "debug.h"
#include "led.h"
#include "common.h"
#include "config.h"

IMU imu(I2C_SENSOR, RCC_I2C_SENSOR);

extern "C" 
int 
main(void)
{
	int i;

	clock_setup();
	gpio_setup();
	debug_setup();
	imu.setup();

	/* Set two LEDs for wigwag effect when toggling. */
	led_set(LED1);
	
	printf("\n\nSkippycopter v1.0\n");


    {
        printf("Checking IMU... ");
        imu_check_t imuCheck = imu.check();

        if (imuCheck == IMU_CHECK_OK)
            puts("OK");
        else
            puts("KO !!");
    }

    imu.init();

	/* Blink the LEDs (PD12, PD13, PD14 and PD15) on the board. */
	while (1) {
        data3 compass;
        imu.hmc5883l.read_data(&compass);

        printf("Compass: %d %d %d\n", compass.x, compass.y, compass.z);

		/* Toggle LEDs. */
		led_toggle(LED1 | LED2);
		//printf(".\n");
		for (i = 0; i < 6000000; i++) { /* Wait a bit. */
			__asm__("nop");
		}
	}

	return 0;
}
