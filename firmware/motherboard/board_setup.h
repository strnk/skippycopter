#ifndef BOARD_SETUP_H
#define BOARD_SETUP_H

#if defined (__cplusplus)
extern "C" {
#endif


// Initialize the clock to 168MHz
void clock_setup(void);

// Initialize the GPIO and their AF
void gpio_setup(void);

#if defined (__cplusplus)
}
#endif

#endif // BOARD_SETUP_H