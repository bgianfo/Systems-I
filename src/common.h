#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdio.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "str.h"

#define INPUTLEN 42

#define DB_FILE_ERROR "Database file error\n"

#define cmps(x,y) strncmp(x, y, strlen(y)) == 0

#define allocate malloc

#define deallocate free


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
