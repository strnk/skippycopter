#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#include "i2c.h"

uint8_t i2c_command;
uint8_t txbuffer[I2C_BUFFER_SIZE];
uint8_t txlen;
uint8_t txptr;
uint8_t rxbuffer[I2C_BUFFER_SIZE];
uint8_t rxlen;

static i2c_rxcmd_t i2c_handler;


void 
I2C_init(uint8_t address, i2c_rxcmd_t handler)
{
	i2c_handler = handler;
	// load address into TWI address register
	TWAR = address << 1;
	// set the TWCR to enable address matching and enable TWI, clear TWINT, enable TWI interrupt
	TWCR = (1<<TWIE) | (1<<TWEA) | (1<<TWINT) | (1<<TWEN);
}

void 
I2C_stop(void)
{
	// clear acknowledge and enable bits
	TWCR &= ~( (1<<TWEA) | (1<<TWEN) );
}

#define i2c_clear_ack() TWCR |= _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWEN)
#define i2c_clear_nack() TWCR |= _BV(TWIE) | _BV(TWINT) | _BV(TWEN)
#define i2c_ack() TWCR |= _BV(TWIE) | _BV(TWEA) | _BV(TWEN)

ISR(TWI_vect){
	
	// temporary stores the received data
	uint8_t data;
	
	if ( (TWSR & 0xF8) == TW_SR_SLA_ACK )
	{ // SLA+W received
		i2c_command = I2C_CMD_RESERVED;


		rxlen = 0;
		txlen = 0;
		txptr = 0xFF;
		// clear TWI interrupt flag, prepare to receive next byte and acknowledge
		TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN); 
	}
	else if ( (TWSR & 0xF8) == TW_SR_DATA_ACK )
	{ // Receiver : data received, ACK returned
		// Data from master
		data = TWDR;
		
		// No command read until now -- sets the command
		if (i2c_command == I2C_CMD_RESERVED){
			i2c_command = data;

			// clear TWI interrupt flag, prepare to receive next byte and acknowledge
			TWCR |= (1<<TWIE) | (1<<TWINT) | (1<<TWEA) | (1<<TWEN); 
		}
		else
		{ // Stores the parameters
			rxbuffer[rxlen++] = data;
			
			// if there is still enough space inside the buffer
			if (rxlen < I2C_BUFFER_SIZE)
				i2c_clear_ack();
			else
				i2c_clear_nack();
		}
	}
	else if ( (TWSR & 0xF8) == TW_SR_STOP )
	{ // STO (receiver)
		i2c_handler(i2c_command, rxlen, rxbuffer, &txlen, txbuffer);
		if (txlen != 0)
			txptr = 0;
		i2c_clear_ack();
	}
	else if ( (TWSR & 0xF8) == TW_ST_SLA_ACK )
	{ // SLA+R received
		if (txptr == 0xFF) 
			TWDR = 0x00;
		else 
			TWDR = txbuffer[txptr++];

		// if there is another buffer address that can be sent
		if(txptr < txlen && txptr != 0xFF)
			i2c_clear_ack();
		else
			i2c_clear_nack();	
	}
	else if ( (TWSR & 0xF8) == TW_ST_DATA_ACK )
	{ // Transmitter: data transmitted, received ACK
		if (txptr == 0xFF) 
			TWDR = 0x00;
		else 
			TWDR = txbuffer[txptr++];

		// if there is another buffer address that can be sent
		if(txptr < txlen && txptr != 0xFF)
			i2c_clear_ack();
		else
			i2c_clear_nack();		
	}
	else
		i2c_ack(); 
}