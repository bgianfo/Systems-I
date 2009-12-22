#ifndef __PROJECT1_H__
#define __PROJECT1_H__

#include "common.h"

inaction_t process( char command[] );

void stats( dbentry* db, inaction_t action );

inaction_t getinput( char* buffer );

#endif
