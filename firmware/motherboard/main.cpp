#include <stdio.h>
#include <imu/imu.h>
#include <periph/sys_scheduler.h>
#include "board_setup.h"
#include "debug.h"
#include "led.h"
#include "common.h"
#include "config.h"

IMU imu(I2C_SENSOR, RCC_I2C_SENSOR);

data3 magneto, accelero, gyro;
int32_t pressure, temperature;
uint32_t main_loop_performance;

void toggle(uint32_t* data) {
	/* Toggle LEDs. */
	led_toggle(LED1 | LED2);
}

void display_data(uint32_t* data) {
	printf("Magneto : %d %d %d\n", magneto.x, magneto.y, magneto.z);
	printf("Accelero: %d %d %d\n", accelero.x, accelero.y, accelero.z);
	printf("Gyro    : %d %d %d\n", gyro.x, gyro.y, gyro.z);
	printf("Pressure: %ld\n", pressure);
	printf("Temp    : %ld\n", temperature);
	printf("Perf    : %lu\n", main_loop_performance);

	main_loop_performance = 0;
}

extern "C" 
int 
main(void)
{
	clock_setup();
	SystemScheduler.init();

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

    SystemScheduler.registerTask(SYS_SCHEDULER_S(1), toggle);
    SystemScheduler.registerTask(SYS_SCHEDULER_S(1), display_data);

	/* Event loop */
	while (1) {
        if (SystemScheduler.pending)
        	SystemScheduler.trigger();

        if (imu.hmc5883l.ready) {
        	imu.hmc5883l.ready = false;
        	magneto = imu.hmc5883l.magneto;
        }

        if (imu.adxl345.ready) {
        	imu.adxl345.ready = false;
        	accelero = imu.adxl345.accelero;
        }

        if (imu.l3g4200d.ready) {
        	imu.l3g4200d.ready = false;
        	gyro = imu.l3g4200d.gyro;
        }

        if (imu.bmp085.ready) {
        	imu.bmp085.ready = false;
        	pressure = imu.bmp085.pressure;
        	temperature = imu.bmp085.temperature;
        }
        main_loop_performance++;
	}

	return 0;
}
