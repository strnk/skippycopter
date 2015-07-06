#ifndef CLI_FUNCTIONS_PWM_H
#define CLI_FUNCTIONS_PWM_H

#include <cli/cli.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t cli_func_pwm_pwmlo(int argc, char **argv);
uint8_t cli_func_pwm_pwmhi(int argc, char **argv);

#ifdef __cplusplus
}
#endif


#endif // CLI_FUNCTIONS_PWM_H