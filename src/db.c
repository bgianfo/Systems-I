

#include "db.h"
#include "common.h"

static void stats( const entry* db, inaction_t action )
{
  entry* current = db; 
  int total_h = 0;
  int total_m = 0;
  int total_s = 0;
  int total_cds = 0;
  int total_tracks = 0;

  while ( current != NULL ) 
  {
    total_s += current->time_s;
    total_m += current->time_m;
    total_cds++;
    total_tacks += current->tracks;

    /* Advance list pointer */
    current = current->artist_next;
  }

  int tmp;

  /* Tally up seconds which could be converted to minutes */
  tmp = total_s / 60;
  total_s = total_s % 60;

  /* Tally up minutes which could be converted to hours */
  total_m += tmp;
  tmp = total_m / 60;
  total_m = total_m % 60;
  total_h += tmp;

  switch (action) {
    case time:
      printf( "total time %d:%d:%d", total_h, total_m, total_s );
      break;
    case count:
      printf( "%d CDs", total_cds );
      break;
    case tracks:
      printf( "%d tracks", total_tracks );
      break;
    case status:
      printf( "%d CDs; %d tracks; total time %d:%d:%d", 
          total_cds, total_tracks, total_h, total_m, total_s ); 
      break;
    default:
      break:
  }
}
