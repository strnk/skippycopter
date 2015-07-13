#ifndef SYSTEM_SYSTEM_H
#define SYSTEM_SYSTEM_H

#include <libopencm3/stm32/rcc.h>
#include <system/powerboard.h>

// sys_check() failure codes
#define SYS_CHECK_OK				0x00
#define SYS_CHECK_POWERBOARD_FAIL	0x01

typedef uint8_t sys_check_t;

class System
{
private:
	uint32_t i2c;
	rcc_periph_clken i2c_rcc;

public:
	Powerboard powerboard;

public:
	System(uint32_t i2c, rcc_periph_clken i2c_rcc)
		: i2c(i2c), i2c_rcc(i2c_rcc), powerboard(i2c) 
		{ }

	void setup();
	void init(void);
	sys_check_t check(void);
};
#endif // SYSTEM_SYSTEM_H