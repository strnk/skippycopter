#ifndef SYS_FLAGS_H
#define SYS_FLAGS_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
	int	data_reporting 	: 1;
	int orientation_reporting : 1;
} cli_flags_t;

#ifndef IN_FLAGS_C
extern cli_flags_t cli_flags;
#endif

void flags_setup(void);

#ifdef __cplusplus
}
#endif

#endif // SYS_FLAGS_H