#ifndef CONFIG_H
#define CONFIG_H
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/cm3/nvic.h>


// GPS port
#define RCC_UART_GPS		RCC_USART1
#define IRQ_UART_GPS		NVIC_USART1_IRQ
#define usart_gps_isr		usart1_isr
#define UART_GPS 			USART1

// Debug port
#define RCC_UART_DEBUG		RCC_USART3
#define IRQ_UART_DEBUG		NVIC_USART3_IRQ
#define usart_debug_isr		usart3_isr
#define UART_DEBUG			USART3

// Telemetry port
#define RCC_UART_TELEMETRY	RCC_UART4
#define IRQ_UART_TELEMETRY	NVIC_UART4_IRQ
#define usart_telemetry_isr uart4_isr
#define UART_TELEMETRY		UART4

// Low PWM group
#define RCC_PWM_LO_TIMER	RCC_TIM2
#define PWM_LO_TIMER		TIM2
#define PWM1_CHANNEL		TIM_OC1
#define	PWM2_CHANNEL		TIM_OC2
#define PWM3_CHANNEL		TIM_OC3
#define	PWM4_CHANNEL		TIM_OC4

// High PWM group
#define RCC_PWM_HI_TIMER	RCC_TIM3
#define PWM_HI_TIMER		TIM3
#define PWM5_CHANNEL		TIM_OC3
#define	PWM6_CHANNEL		TIM_OC1
#define PWM7_CHANNEL		TIM_OC2
#define	PWM8_CHANNEL		TIM_OC4

// System I2C
#define RCC_I2C_SYSTEM		RCC_I2C3
#define I2C_SYSTEM			I2C3

// Sensor I2C
#define RCC_I2C_SENSOR		RCC_I2C1
#define I2C_SENSOR			I2C1

// LEDs GPIOs
#define LEDS_PORT			GPIOB
#define LED1_GPIO			GPIO12
#define LED2_GPIO			GPIO13


// Performance counters
#define PERF_MAIN_LOOP		0
#define PERF_IMU_ACCEL		1
#define PERF_IMU_GYRO		2
#define PERF_IMU_PRESS		3
#define PERF_IMU_MAGN		4

#endif // CONFIG_H