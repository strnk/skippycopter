#ifndef CONFIG_H
#define CONFIG_H

#define ADC_INTENSITY	0x02
#define ADC_VOLTAGE		0x00

#define LED_BATT		7

// Powerboard address on the system I2C
#define PWBOARD_ADDR	0x42

// Time (ms) between voltage/intensity samples
#define TIME_BETWEEN_SAMPLES	200

// Size of the array for average calculation
#define AVG_VALUES	32	// Average on 6,4sec

#endif // CONFIG_H