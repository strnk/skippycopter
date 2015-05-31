#ifndef LED_H
#define LED_H
#include <inttypes.h>

#if defined (__cplusplus)
extern "C" {
#endif

#define LED1	0x01
#define LED2	0X02

void led_set(uint8_t leds);
void led_clear(uint8_t leds);
void led_toggle(uint8_t leds);

#if defined (__cplusplus)
}
#endif

#endif // LED_H