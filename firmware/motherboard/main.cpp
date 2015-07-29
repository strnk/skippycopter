#include <stdio.h>
#include <imu/imu.h>
#include <periph/sys_scheduler.h>
#include <debug/performance.h>
#include <periph/pwm.h>
#include <periph/ppm.h>
#include <cli/cli.h>
#include <system/system.h>
#include <radio/radio.h>
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
PPM ppm(PPM_TIMER, 6);
Radio radio;

void toggle(__attribute__((unused)) uint32_t* data) {
	/* Toggle LEDs. */
	led_toggle(LED1 | LED2);
	perf_validate();
}

void display_data(__attribute__((unused)) uint32_t* data) {
	if (FLAG(data_reporting)) {
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

    if (FLAG(orientation_reporting)) {
        orientation_t orientation = imu.getOrientation();

        printf("YAW: %03.2f  PITCH: %03.2f  ROLL: %03.2f\n", orientation.yaw, orientation.pitch, orientation.roll);
    }
}

void msg_periodic_fastest(__attribute__((unused)) uint32_t* data) {
    if (FLAG(msg_at)) {
        orientation_t orientation = imu.getOrientation();

        printf("$AT %08X %08X %08X\n", floatAsU32(orientation.yaw), floatAsU32(orientation.pitch), floatAsU32(orientation.roll));
    }
}

void msg_periodic_fast(__attribute__((unused)) uint32_t* data) {
    // Nothing ATM
}

void msg_periodic_slow(__attribute__((unused)) uint32_t* data) {
    if (FLAG(msg_pow)) {
        printf("$POW %08X %08X\n", floatAsU32(sys.powerboard.voltage_avg), floatAsU32(sys.powerboard.intensity_avg));
    }
}

void msg_periodic_slowest(__attribute__((unused)) uint32_t* data) {
    // Nothing ATM
}

extern "C" 
int 
main(void)
{
	clock_setup();
	SystemScheduler.init();

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

    // Initialize PWMs with 20ms period
    pwm_lo.init(84, 20000);
    pwm_hi.init(84, 20000);

    // Initialize the motors
    pwm_lo.arm(PWM1_CHANNEL);
    pwm_lo.arm(PWM2_CHANNEL);
    pwm_lo.arm(PWM3_CHANNEL);
    pwm_lo.arm(PWM4_CHANNEL);

    pwm_lo.set_pulse_width(PWM1_CHANNEL, 1060);
    pwm_lo.set_pulse_width(PWM2_CHANNEL, 1060);
    pwm_lo.set_pulse_width(PWM3_CHANNEL, 1060);
    pwm_lo.set_pulse_width(PWM4_CHANNEL, 1060);

#ifndef USE_PPM_INPUT
    pwm_hi.arm(PWM5_CHANNEL);
#endif
    pwm_hi.arm(PWM6_CHANNEL);
    pwm_hi.arm(PWM7_CHANNEL);
    pwm_hi.arm(PWM8_CHANNEL);

    // Initialize PPM with 20ms period
    ppm.init(PPM_CHANNEL, PPM_TIMER_INPUT);

    // Set up the system tasks
    SystemScheduler.registerTask(SYS_SCHEDULER_S(1), toggle);
    SystemScheduler.registerTask(SYS_SCHEDULER_S(1), display_data);

    // System message handlers
    SystemScheduler.registerTask(SYS_SCHEDULER_MS(10), msg_periodic_fastest);
    SystemScheduler.registerTask(SYS_SCHEDULER_MS(100), msg_periodic_fast);
    SystemScheduler.registerTask(SYS_SCHEDULER_MS(500), msg_periodic_slow);
    SystemScheduler.registerTask(SYS_SCHEDULER_S(1), msg_periodic_slowest);

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

        if (ppm.pending) {
            radio.update_from_ppm((uint32_t*)ppm.pulses);
            ppm.pending = false;

            if (radio.cmd.aux1 < 50.0) {
                pwm_lo.set_throttle(PWM1_CHANNEL, 0);
            }
            else {
                pwm_lo.set_throttle(PWM1_CHANNEL, (uint16_t)(radio.cmd.throttle * 10));
            }
        }

        imu.checkPending();
	}

	return 0;
}
