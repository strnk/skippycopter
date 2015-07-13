#ifndef ADC_H
#define ADC_H

#include <inttypes.h>

void ADC_init(void);
uint16_t ADC_read(uint8_t channel);

#endif // ADC_H