#if ( !defined(SYS_FLAGS_H) || defined(GENERATE_FLAG_STRINGS) )

#if (!defined(GENERATE_FLAG_STRINGS))
#define SYS_FLAGS_H
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/flagpp.h>

BEGIN_FLAGS()
    DECL_FLAG(data_reporting);
    DECL_FLAG(orientation_reporting);
    DECL_FLAG(msg_at);
    DECL_FLAG(msg_gps);
    DECL_FLAG(msg_pow);
END_FLAGS()


#ifdef __cplusplus
}
#endif

#endif // (!defined(SYS_FLAGS_H) || defined(GENERATE_FLAG_STRINGS))