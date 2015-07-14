#define IN_FLAGS_C
#include <sys/flags.h>

cli_flags_t cli_flags;

void 
flags_setup(void)
{
	cli_flags.data_reporting = 0;
	cli_flags.orientation_reporting = 0;
	cli_flags.raw_attitude = 0;
}