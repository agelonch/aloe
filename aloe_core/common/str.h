#ifndef STR_H
#define STR_H

#include <string.h>

#define STR_LEN 64
#define LSTR_LEN 512

#define strdef(a) char a[STR_LEN]
#define lstrdef(a) char a[LSTR_LEN]
#define lstring char*
#define string char*

#define strcpy(a,b) strncpy(a,b,STR_LEN)
#define lstrcpy(a,b) strncpy(a,b,LSTR_LEN)
#define strcmp(a,b) strncmp(a,b,STR_LEN)
#define lstrcmp(a,b) strncmp(a,b,LSTR_LEN)

#endif
