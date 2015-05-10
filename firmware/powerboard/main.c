#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>

void
main(void) 
{
    // Low battery LED
    DDRD = (1 << 7);
    
    while (1)
    {
        PORTD |= (1 << 7);
        _delay_ms(1000);
        PORTD &= ~(1 << 7);
        _delay_ms(1000);
    }
}
