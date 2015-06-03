#include <stdio.h>
#include <imu/imu.h>
#include <periph/sys_scheduler.h>
#include "board_setup.h"
#include "debug.h"
#include "led.h"
#include "common.h"
#include "config.h"

IMU imu(I2C_SENSOR, RCC_I2C_SENSOR);

uint8_t data_received;
data3 magneto, accelero, gyro;
int32_t pressure, temperature;
uint16_t main_loop_performance, loop_perf;

void toggle(uint32_t* data) {
	/* Toggle LEDs. */
	led_toggle(LED1 | LED2);
	loop_perf = main_loop_performance;
	main_loop_performance = 0;
}

void display_data(uint32_t* data) {
	
	puts("");
	// in mG (gauss)
	printf("Magneto : %.2f %.2f %.2f\n", magneto.x*0.92, magneto.y*0.92, magneto.z*0.92);

	// in G (accel)
	printf("Accelero: %.2f %.2f %.2f\n", accelero.x*0.04, accelero.y*0.04, accelero.z*0.04);

	// in degrees per sec
	printf("Gyro    : %.2f %.2f %.2f\n", gyro.x*0.07, gyro.y*0.07, gyro.z*0.07);

	// in hPa
	printf("Pressure: %.2f\n", pressure/100.0);

	// in °C
	printf("Temp    : %.1f\n", temperature/10.0);
	printf("Perf    : %lu\n", main_loop_performance);
	
	/*
	printf("%04X %04X %04X %04X %04X %04X %04X %04X %04X %08X %08X %08X\n", 
		magneto.x, magneto.y, magneto.z,
		accelero.x, accelero.y, accelero.z,
		gyro.x, gyro.y, gyro.z,
		pressure, temperature,
		main_loop_performance); */
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
    //SystemScheduler.registerTask(SYS_SCHEDULER_S(1), display_data);
    data_received = 0x00;

	/* Event loop */
	while (1) {
        if (SystemScheduler.pending)
        	SystemScheduler.trigger();

        if (imu.hmc5883l.ready) {
        	imu.hmc5883l.ready = false;
        	magneto = imu.hmc5883l.magneto;
        	data_received |= 0x04;
        }

        if (imu.adxl345.ready) {
        	imu.adxl345.ready = false;
        	accelero = imu.adxl345.accelero;
        	data_received |= 0x01;
        }

        if (imu.l3g4200d.ready) {
        	imu.l3g4200d.ready = false;
        	gyro = imu.l3g4200d.gyro;
        	data_received |= 0x02;
        }

        if (imu.bmp085.ready) {
        	imu.bmp085.ready = false;
        	pressure = imu.bmp085.pressure;
        	temperature = imu.bmp085.temperature;
        }

        if (data_received == 0x03) {
			printf("%04X %04X %04X %04X %04X %04X %04X %04X %04X %08X %04X %04X\n", 
				0, 0, 0,
				accelero.x&0xFFFF, accelero.y&0xFFFF, accelero.z&0xFFFF,
				gyro.x&0xFFFF, gyro.y&0xFFFF, gyro.z&0xFFFF,
				pressure, temperature&0xFFFF, loop_perf);
        	data_received = 0x00;
        } else if (data_received == 0x07) {
			printf("%04X %04X %04X %04X %04X %04X %04X %04X %04X %08X %04X %04X\n", 
				magneto.x&0xFFFF, magneto.y&0xFFFF, magneto.z&0xFFFF,
				accelero.x&0xFFFF, accelero.y&0xFFFF, accelero.z&0xFFFF,
				gyro.x&0xFFFF, gyro.y&0xFFFF, gyro.z&0xFFFF,
				pressure, temperature&0xFFFF, loop_perf);
        	data_received = 0x00;
        }
        main_loop_performance++;
	}

	return 0;
}
