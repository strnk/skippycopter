#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/power.h>
#include <avr/interrupt.h>

#include "adc.h"
#include "i2c.h"
#include "config.h"

uint16_t raw_intensity;
uint16_t raw_voltage;


typedef struct 
{
    uint16_t array[AVG_VALUES];
    uint16_t mean;
    uint8_t ptr;
} uint16_avg_t;

uint16_avg_t avg_intensity;
uint16_avg_t avg_voltage;

struct 
{
    int led_critical : 1;
} flags; 

void
i2c_handler(uint8_t cmd, uint8_t len, uint8_t *data, uint8_t *txlen, uint8_t *txdata)
{
	switch (cmd)
	{
		// LED_CTRL
		case 0x00:
			if (len && data[0] == 1) {
        		PORTD |= _BV(LED_BATT);
                flags.led_critical = 0;
            }
        	else if (len && data[0] == 0) {
        		PORTD &= ~ _BV(LED_BATT);
                flags.led_critical = 0;
            }
            else if (len && data[0] == 2)
                flags.led_critical = 1;
        	break;

        // READ_VOLTAGE
        case 0x01:
        	*txlen = 2;
        	txdata[1] = raw_voltage & 0xFF;
        	txdata[0] = (raw_voltage & 0xFF00) >> 8;
        	break;

        // READ_INTENSITY
        case 0x02:
        	*txlen = 2;
        	txdata[1] = raw_intensity & 0xFF;
        	txdata[0] = (raw_intensity & 0xFF00) >> 8;
        	break;

        // READ_MEAN_VOLTAGE
        case 0x03:
            *txlen = 2;
            txdata[1] = avg_voltage.mean & 0xFF;
            txdata[0] = (avg_voltage.mean & 0xFF00) >> 8;
            break;

        // READ_MEAN_INTENSITY
        case 0x04:
            *txlen = 2;
            txdata[1] = avg_intensity.mean & 0xFF;
            txdata[0] = (avg_intensity.mean & 0xFF00) >> 8;
            break;

        // DEVID
        case 0x80:
        	*txlen = 1;
        	txdata[0] = 0x42;
        	break;
        	
		default:
		case I2C_CMD_RESERVED:
			*txlen = 0;
		break;
	}
}

void
avg_init(uint8_t adc_channel, uint16_t *raw, uint16_avg_t *avg)
{
    int i;

    avg->ptr = 1;
    *raw = ADC_read(adc_channel);

    for (i = 0; i < AVG_VALUES; i++)
        avg->array[i] = *raw;

    avg->mean = *raw;
}

void
avg_compute(uint8_t adc_channel, uint16_t *raw, uint16_avg_t *avg)
{
    uint32_t acc = 0;
    int i;

    *raw = ADC_read(adc_channel);
    avg->array[avg->ptr] = *raw;
    avg->ptr++;

    if (avg->ptr >= AVG_VALUES)
        avg->ptr = 0;

    for (i = 0; i < AVG_VALUES; i++)
        acc += avg->array[i];

    avg->mean = acc / AVG_VALUES;
}

void
main(void) 
{
	// Clock prescaler =1 => CLK = 8MHz
	clock_prescale_set(clock_div_1);

    // Low battery LED
    DDRD = _BV(LED_BATT);

    // Disable the digital input buffer on PA0-7
    DIDR0 = 0xFF;
    // Disable the pull-ups on PA0-7, set PA0-7 as inputs
    DDRA = 0x00;
    PORTA = 0x00;

    ADC_init();
    I2C_init(PWBOARD_ADDR, i2c_handler);

   	sei();

    avg_init(ADC_INTENSITY, &raw_intensity, &avg_intensity);
    avg_init(ADC_VOLTAGE, &raw_voltage, &avg_voltage);

    // Initialize the average arrays
    while (1)
    {    
        avg_compute(ADC_INTENSITY, &raw_intensity, &avg_intensity);
        avg_compute(ADC_VOLTAGE, &raw_voltage, &avg_voltage);
    	_delay_ms(TIME_BETWEEN_SAMPLES);
    }
}
