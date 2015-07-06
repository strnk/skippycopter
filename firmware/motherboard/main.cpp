#include <stdio.h>
#include <imu/imu.h>
#include <periph/sys_scheduler.h>
#include <debug/performance.h>
#include <periph/pwm.h>
#include <cli/cli.h>
#include "board_setup.h"
#include "led.h"
#include "common.h"
#include "config.h"

IMU imu(I2C_SENSOR, RCC_I2C_SENSOR);
PWM pwm_lo(PWM_LO_TIMER, 1060, 1860);
PWM pwm_hi(PWM_HI_TIMER, 1060, 1860);

uint8_t data_received;
data3 magneto, accelero, gyro;
int32_t pressure, temperature;

void toggle(uint32_t* data) {
	/* Toggle LEDs. */
	led_toggle(LED1 | LED2);
	perf_validate();
}

void display_data(uint32_t* data) {
	
	puts("");
	// in mG (gauss)
	printf("Magneto : %.2f %.2f %.2f\n", magneto.x*0.92, magneto.y*0.92, magneto.z*0.92);

	// in G (accel)
	printf("Accelero: %.2f %.2f %.2f\n", accelero.x*0.008, accelero.y*0.008, accelero.z*0.008);

	// in degrees per sec
	printf("Gyro    : %.2f %.2f %.2f\n", gyro.x*0.07, gyro.y*0.07, gyro.z*0.07);

	// in hPa
	printf("Pressure: %.2f\n", pressure/100.0);

	// in °C
	printf("Temp    : %.1f\n", temperature/10.0);
	printf("Perf    : M:%lu ACC: %lu GYR: %lu MAG: %lu PRS: %lu\n", 
		perf_counter_value(PERF_MAIN_LOOP), perf_counter_value(PERF_IMU_ACCEL),
		perf_counter_value(PERF_IMU_GYRO), perf_counter_value(PERF_IMU_MAGN),
		perf_counter_value(PERF_IMU_PRESS));
}

extern "C" 
int 
main(void)
{
	clock_setup();
	SystemScheduler.init();

	gpio_setup();
	CLIHandler.init();
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

    pwm_lo.init(84, 20000);

    // Initialize the motors
    pwm_lo.arm(PWM1_CHANNEL);
    pwm_lo.arm(PWM2_CHANNEL);
    pwm_lo.arm(PWM3_CHANNEL);
    pwm_lo.arm(PWM4_CHANNEL);

    pwm_lo.set_pulse_width(PWM1_CHANNEL, 1060);
    pwm_lo.set_pulse_width(PWM2_CHANNEL, 1060);
    pwm_lo.set_pulse_width(PWM3_CHANNEL, 1060);
    pwm_lo.set_pulse_width(PWM4_CHANNEL, 1060);

    SystemScheduler.registerTask(SYS_SCHEDULER_S(1), toggle);
    //SystemScheduler.registerTask(SYS_SCHEDULER_S(1), display_data);
    data_received = 0x00;

    // Display the prompt on the debug interface
    CLIHandler.prompt();

	/* Event loop */
	while (1) {
        perf_counter_inc(PERF_MAIN_LOOP);

        if (SystemScheduler.pending)
        	SystemScheduler.trigger();

        if (CLIHandler.pending)
        	CLIHandler.trigger();

        if (imu.hmc5883l.ready) {
        	imu.hmc5883l.ready = false;
        	magneto = imu.hmc5883l.magneto;
        	data_received |= 0x04;
        	perf_counter_inc(PERF_IMU_MAGN);
        }

        if (imu.adxl345.ready) {
        	imu.adxl345.ready = false;
        	accelero = imu.adxl345.accelero;
        	data_received |= 0x01;
        	perf_counter_inc(PERF_IMU_ACCEL);
        }

        if (imu.l3g4200d.ready) {
        	imu.l3g4200d.ready = false;
        	gyro = imu.l3g4200d.gyro;
        	data_received |= 0x02;
        	perf_counter_inc(PERF_IMU_GYRO);
        }

        if (imu.bmp085.ready) {
        	imu.bmp085.ready = false;
        	pressure = imu.bmp085.pressure;
        	temperature = imu.bmp085.temperature;
        	perf_counter_inc(PERF_IMU_PRESS);
        }

        /*
        if (data_received == 0x03) {
			printf("%04X %04X %04X %04X %04X %04X %04X %04X %04X %08X %04X %04X\n", 
				0, 0, 0,
				accelero.x&0xFFFF, accelero.y&0xFFFF, accelero.z&0xFFFF,
				gyro.x&0xFFFF, gyro.y&0xFFFF, gyro.z&0xFFFF,
				pressure, temperature&0xFFFF, perf_counter_value(PERF_MAIN_LOOP));
        	data_received = 0x00;
        } else if (data_received == 0x07) {
			printf("%04X %04X %04X %04X %04X %04X %04X %04X %04X %08X %04X %04X\n", 
				magneto.x&0xFFFF, magneto.y&0xFFFF, magneto.z&0xFFFF,
				accelero.x&0xFFFF, accelero.y&0xFFFF, accelero.z&0xFFFF,
				gyro.x&0xFFFF, gyro.y&0xFFFF, gyro.z&0xFFFF,
				pressure, temperature&0xFFFF, perf_counter_value(PERF_MAIN_LOOP));
        	data_received = 0x00;
        }*/
	}

	return 0;
}
