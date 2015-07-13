#include <avr/io.h>
#include "adc.h"

void
ADC_init(void)
{
	// ADC setup
  	// 8 prescaler => ADC clock=125kHz
    ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1);

   	// Reference: AVCC (5V)
   	ADMUX = _BV(REFS0);
}

uint16_t 
ADC_read(uint8_t channel)
{
	uint16_t value;

	// Change the channel value
	ADMUX &= (_BV(REFS1) | _BV(REFS0) | _BV(ADLAR));
	ADMUX |= channel; 

	// Start the conversion
	ADCSRA |= _BV(ADSC);
	
	// Wait the end of the conversion
	while (ADCSRA & _BV(ADSC));

	value = ADCL;
	value |= (ADCH<<8);
	return value;
}