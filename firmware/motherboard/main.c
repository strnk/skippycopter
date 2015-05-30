#include <stdio.h>
#include "board_setup.h"
#include "debug.h"
#include "led.h"
#include "imu.h"
#include "common.h"
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
	
	printf("\n\nSkippycopter v1.0\n");


    {
        printf("Checking IMU... ");
        imu_check_t imuCheck = imu_check();

        if (imuCheck == IMU_CHECK_OK)
            puts("OK");
        else
            puts("KO !!");
    }

    imu_init();

	/* Blink the LEDs (PD12, PD13, PD14 and PD15) on the board. */
	while (1) {
        data3 compass;
        hmc5883l_read_data(&compass);

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
