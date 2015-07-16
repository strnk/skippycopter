#include <stdio.h>
#include <imu/imu.h>
#include <periph/sys_scheduler.h>
#include <debug/performance.h>
#include <periph/pwm.h>
#include <cli/cli.h>
#include <system/system.h>
#include <sys/flags.h>
#include <gps/gps.h>
#include "board_setup.h"
#include "led.h"
#include "common.h"
#include "config.h"

System sys(I2C_SYSTEM, RCC_I2C_SYSTEM);
IMU imu(I2C_SENSOR, RCC_I2C_SENSOR);
PWM pwm_lo(PWM_LO_TIMER, 1060, 1860);
PWM pwm_hi(PWM_HI_TIMER, 1060, 1860);

void toggle(uint32_t* data) {
	/* Toggle LEDs. */
	led_toggle(LED1 | LED2);
	perf_validate();
}

void display_data(uint32_t* data) {
	if (cli_flags.data_reporting) {
    	puts("");
    	// in mG (gauss)
    	printf("Magneto : %.2f %.2f %.2f\n", imu.magneto.x, imu.magneto.y, imu.magneto.z);

    	// in G (accel)
    	printf("Accelero: %.2f %.2f %.2f\n", imu.accelero.x, imu.accelero.y, imu.accelero.z);

    	// in degrees per sec
    	printf("Gyro    : %.2f %.2f %.2f\n", imu.gyro.x, imu.gyro.y, imu.gyro.z);

    	// in hPa
    	printf("Pressure: %.2f\n", imu.pressure);

    	// in °C
    	printf("Temp    : %.1f\n", imu.temperature);
    	printf("Perf    : M:%lu ACC: %lu GYR: %lu MAG: %lu PRS: %lu\n", 
    		perf_counter_value(PERF_MAIN_LOOP), perf_counter_value(PERF_IMU_ACCEL),
    		perf_counter_value(PERF_IMU_GYRO), perf_counter_value(PERF_IMU_MAGN),
    		perf_counter_value(PERF_IMU_PRESS));
    }

    if (cli_flags.orientation_reporting) {
        orientation_t orientation = imu.getOrientation();

        printf("YAW: %03.2f  PITCH: %03.2f  ROLL: %03.2f\n", orientation.yaw, orientation.pitch, orientation.roll);
    }
}

void display_raw_data(uint32_t* data) {
    orientation_t orientation;
    uint32_t* yaw = (uint32_t*)(&orientation.yaw);
    uint32_t* pitch = (uint32_t*)(&orientation.pitch); 
    uint32_t* roll = (uint32_t*)(&orientation.roll);  
    orientation = imu.getOrientation();

    if (cli_flags.raw_attitude) {
        printf("AT %04X %04X %04X\n", *yaw, *pitch, *roll);
    }
}

extern "C" 
int 
main(void)
{
	clock_setup();
	SystemScheduler.init();

    flags_setup();
	gpio_setup();
	CLIHandler.init();
    sys.setup();
	imu.setup();

	led_set(LED1);
	printf("\n\nSkippycopter v1.0\n");

    // Wait for the powerboard to initialize
    for (unsigned long i = 0; i < 42000000L; i++)
        asm("nop");

    {
        printf("Checking system... \n");
        sys_check_t sysCheck = sys.check();

        if (sysCheck == SYS_CHECK_OK)
            puts("OK");
        else
            puts("KO !!");
    }

    sys.init();

    {
        printf("Checking IMU... \n");
        imu_check_t imuCheck = imu.check();

        if (imuCheck == IMU_CHECK_OK)
            puts("OK");
        else
            puts("KO !!");
    }

    imu.init();

    GPSHandler.init();

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
    SystemScheduler.registerTask(SYS_SCHEDULER_S(1), display_data);
    SystemScheduler.registerTask(SYS_SCHEDULER_MS(10), display_raw_data);

    // Display the prompt on the debug interface
    CLIHandler.prompt();

	/* Event loop */
	while (1) {
        perf_counter_inc(PERF_MAIN_LOOP);

        if (SystemScheduler.pending)
        	SystemScheduler.trigger();

        if (CLIHandler.pending)
        	CLIHandler.trigger();

        if (GPSHandler.pending)
            GPSHandler.trigger();

        imu.checkPending();
	}

	return 0;
}
