#ifndef __DB_H__
#define __DB_H__

#include "common.h"

#define MAX_ARTIST_LEN 26
#define MAX_TITLE_LEN  41
#define MAX_TRACK_LIMIT 4
#define MAX_TIME_LIMIT 7
#define DB_INPUT_LIMIT 128

#define HEADER 99

typedef struct dbentry_s {
  unsigned char time_m;
  unsigned char time_s;
  unsigned char tracks;
  struct dbentry_s* title_next;
  struct dbentry_s* artist_next;
  char *title;
  char *artist;
} dbentry;

dbentry* read_db( char *filename );

void deallocate_db( dbentry* db );

void print_action(dbentry* head, inaction_t);

void printrule( int n, inaction_t act );

int title_comp( const void * elem1, const void * elem2 );

int artist_comp( const void * elem1, const void * elem2 );

int length( dbentry *list );

void sort( dbentry** alist, dbentry** tlist );

#endif
