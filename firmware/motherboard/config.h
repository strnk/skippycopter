#ifndef CONFIG_H
#define CONFIG_H
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/i2c.h>
#include <libopencm3/stm32/rcc.h>


// GPS port
#define RCC_UART_GPS		RCC_USART1
#define UART_GPS 			USART1

// Debug port
#define RCC_UART_DEBUG		RCC_USART3
#define UART_DEBUG			USART3

// Telemetry port
#define RCC_UART_TELEMETRY	RCC_UART4
#define UART_TELEMETRY		UART4

// Low PWM group
#define RCC_PWM_LO_TIMER	RCC_TIM2
#define PWM_LO_TIMER		TIM2
#define PWM1_CHANNEL		1
#define	PWM2_CHANNEL		2
#define PWM3_CHANNEL		3
#define	PWM4_CHANNEL		4

// High PWM group
#define RCC_PWM_HI_TIMER	RCC_TIM3
#define PWM_HI_TIMER		TIM3
#define PWM5_CHANNEL		3
#define	PWM6_CHANNEL		1
#define PWM7_CHANNEL		2
#define	PWM8_CHANNEL		4

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

#endif // CONFIG_H