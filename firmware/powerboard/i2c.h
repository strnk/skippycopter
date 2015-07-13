#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

#include <inttypes.h>

#define I2C_CMD_RESERVED	0xFF

#define I2C_BUFFER_SIZE		128

typedef void (*i2c_rxcmd_t)(uint8_t cmd, uint8_t len, uint8_t *data, uint8_t *txlen, uint8_t *txdata);

void I2C_init(uint8_t address, i2c_rxcmd_t handler);
void I2C_stop(void);

#endif // I2C_SLAVE_H