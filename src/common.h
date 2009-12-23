#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define INPUTLEN 42

#define DB_FILE 1
#define DB_FILE_ERROR "Database file error\n"
#define DB_TRUNK_ERROR "Database too large - truncating\n"
#define ALLOC_ERROR "Out of memory - program exiting\n"

#define cmps(x,y) strncmp(x, y, strlen(x)) == 0

#define max( a, b ) ( ((a) > (b)) ? (a) : (b) )

#define min( a, b ) ( ((a) < (b)) ? (a) : (b) )

#define allocate malloc

#define unallocate free

typedef enum {
   noop = 0,
   time,
   quit,
   count,
   tracks,
   status,
   artists,
   titles,
   artist_search,
   title_search
} inaction_t;

#endif
