#include <libopencm3/stm32/gpio.h>
#include "led.h"
#include "config.h"


void led_set(uint8_t leds)
{
	uint16_t gpios = ((leds & LED1) ? LED1_GPIO : 0) | ((leds & LED2) ? LED2_GPIO : 0);
	gpio_set(LEDS_PORT, gpios);
}

void led_clear(uint8_t leds)
{
	uint16_t gpios = ((leds & LED1) ? LED1_GPIO : 0) | ((leds & LED2) ? LED2_GPIO : 0);
	gpio_clear(LEDS_PORT, gpios);
}

void led_toggle(uint8_t leds)
{
	uint16_t gpios = ((leds & LED1) ? LED1_GPIO : 0) | ((leds & LED2) ? LED2_GPIO : 0);
	gpio_toggle(LEDS_PORT, gpios);
}
