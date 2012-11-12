#include <assert.h>
#include <stdio.h>

#define WHERESTR  "[file %s, line %d]: "
#define WHEREARG  __FILE__, __LINE__
#define DEBUGPRINT2(...)       fprintf(stderr, __VA_ARGS__)
#define aerror_msg(_fmt, ...)  DEBUGPRINT2(WHERESTR _fmt, WHEREARG, __VA_ARGS__)
#define aerror(a)  DEBUGPRINT2(WHERESTR a "\n", WHEREARG)

#define ainfo printf
#define awarn printf

#define poserror(a,msg) fprintf(stderr, WHERESTR "%s: \n" msg, WHEREARG, strerror(a))

#ifdef __GNUC__
#  define UNUSED(x) UNUSED_ ## x __attribute__((__unused__))
#define SUPPRESS_NOT_USED_WARN __attribute__ ((unused))
#else
#  define UNUSED(x) UNUSED_ ## x
#endif

#ifdef __GNUC__
#  define UNUSED_FUNCTION(x) __attribute__((__unused__)) UNUSED_ ## x
#else
#  define UNUSED_FUNCTION(x) UNUSED_ ## x
#endif


#define CAST(dst,src,type) type dst = (type) src

