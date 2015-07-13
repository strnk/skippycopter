#ifndef CLI_FUNCTIONS_SYSTEM_H
#define CLI_FUNCTIONS_SYSTEM_H

#include <cli/cli.h>

#ifdef __cplusplus
extern "C" {
#endif

uint8_t cli_func_system_ping(int argc, char **argv);
uint8_t cli_func_system_power(int argc, char **argv);

#ifdef __cplusplus
}
#endif

#endif // CLI_FUNCTIONS_SYSTEM_H