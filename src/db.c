#include "db.h"
#include "common.h"
#include "allocate.h"

dbentry* read_db(char *filename)
{
  FILE *file_fd = fopen( filename, "r" );
  dbentry* head;
  if (file_fd == NULL) {
    fputs(DB_FILE_ERROR,stderr);
    return NULL;
  } else {

    bool first = true;
    dbentry* curr = NULL;
    char buffer[DB_INPUT_LIMIT];
    while ( !feof(file_fd) )
    {
      fgets(buffer, MAX_ARTIST_LEN, file_fd);
      char *artist = trim(buffer);

      fgets(buffer, MAX_TITLE_LEN, file_fd);
      char *title = trim(buffer);

      fgets(buffer, MAX_TRACK_LIMIT, file_fd);
      char ntracks = (char) atoi(buffer);

      fgets(buffer, MAX_TIME_LIMIT, file_fd);
      int time_m = 0;
      int time_s = 0;
      sscanf(buffer,"%d:%d", &time_m, &time_s);

      curr = (dbentry*) allocate(sizeof(dbentry));

      if (first) {
        head = curr;
        first = false;
      }

      curr->artist = artist;
      curr->title = title;
      curr->tracks = ntracks;
      curr->time_m = time_m;
      curr->time_s = time_s;

      curr = curr->artist_next;
    }
  }

  fclose(file_fd);
  return head;
}



int title_comp( const void * elem1, const void * elem2 )
{
  dbentry* dbelem1 = (dbentry*) elem1;
  dbentry* dbelem2 = (dbentry*) elem2;
  return strcmp(dbelem1->title, dbelem2->title);
}


int artist_comp( const void * elem1, const void * elem2 )
{
  dbentry* dbelem1 = (dbentry*) elem1;
  dbentry* dbelem2 = (dbentry*) elem2;
  return strcmp(dbelem1->artist, dbelem2->artist);
}

int length( dbentry *list ) {
  dbentry* cur = list;
  int len = 0;
  while ( NULL != cur )
  {
    ++len;
    cur = cur->artist_next;
  }
  return len;
}


static void sort( dbentry** alist, dbentry** tlist )
{
  dbentry* curr = *alist;

  int len = length( curr );
  int *dbItems = (int*) allocate(len * sizeof(int));
  for( int i = 0; NULL != curr; i++ ) {
    dbItems[i] = (int) curr;
    curr = curr->artist_next;
  }

  // sort items by artist name 
  qsort(dbItems, len, sizeof(int), artist_comp);

  dbentry* ahead = (dbentry*) dbItems[0];
  
  // reconstruct artist links from newly sorted values
  curr = (dbentry*) dbItems[0];
  for( int i = 1; i < len; i++ ) {
    curr->artist_next = (dbentry*) dbItems[i];
    curr = curr->artist_next;
  }


  // now sort
  qsort(dbItems, len, sizeof(int), title_comp);
  dbentry* thead = (dbentry*) dbItems[0];

  // reconstruct title links from newly sorted values
  curr = (dbentry*) dbItems[0];
  for( int i = 1; i < len; i++ ) {
    curr->artist_next = (dbentry*) dbItems[i];
    curr = curr->artist_next;
  }

  *alist = ahead;
  *tlist = thead;
  return;
}
