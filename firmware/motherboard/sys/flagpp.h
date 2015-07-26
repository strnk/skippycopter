// Ugly flag pre-processor ;-)

#undef DECL_FLAG
#undef BEGIN_FLAGS
#undef END_FLAGS

#ifndef GENERATE_FLAG_STRINGS
	#define FLAG(flag) (system_flags.flag)
    #define DECL_FLAG( flag ) unsigned int flag : 1
    #define BEGIN_FLAGS() typedef struct {
    #define END_FLAGS() } system_flags_t; \
            unsigned int setFlagValue(char* flagStr, unsigned int value); \
            extern system_flags_t system_flags;
#else
    #define DECL_FLAG( flag ) else if (!strcmp(flagStr, #flag)) system_flags.flag = value
    #define BEGIN_FLAGS() system_flags_t system_flags; unsigned int setFlagValue(char* flagStr, unsigned int value) { \
            if (!strcmp(flagStr, "")) { return 0; }
    #define END_FLAGS() { return 0; } return 1; }
#endif
