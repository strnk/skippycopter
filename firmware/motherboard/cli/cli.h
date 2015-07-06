#ifndef CLI_H
#define CLI_H

#include <inttypes.h>
#include <stdlib.h>

#define CLI_CMD_MAXLEN 	32 		//!< Maximum length of a command name
#define CLI_CMD_LINE_MAXLEN	128	//!< Maximum length of a command line
#define CLI_CMD_LINE_MAXARGS 16 //!< Maximum number of arguments on the command line


typedef uint8_t (*cli_funcptr_t)(int argc, char **argv);

typedef struct 
{
	const char			cmd[CLI_CMD_MAXLEN];	// Root name of the function
	uint8_t				req_args;				// Number of required arguments
	const cli_funcptr_t	funcptr;				// Pointer
} cli_funcdesc_t;

#ifdef __cplusplus

class CommandLineInterface
{
private:
	const cli_funcdesc_t* funcs;
	uint8_t _argvize(char *str);

public:
	volatile bool pending;


	CommandLineInterface(const cli_funcdesc_t* funcPtr);

	void init(void);
	void prompt(void);
	void trigger(void);
};

#ifndef CLI_NO_EXTERN
extern CommandLineInterface CLIHandler;
#endif

#endif // __cplusplus

#endif // CLI_H