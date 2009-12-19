#include "db.h"
#include "common.h"
#include "allocate.h"

dbentry* read_db(char *filename)
{
  FILE *file_fd = fopen( filename, "r" );
  dbentry* head = NULL;
  if (file_fd == NULL) {
    fputs(DB_FILE_ERROR,stderr);
    return NULL;
  } else {

    bool first = true;
    dbentry* curr = NULL;
    char *buffer = allocate(DB_INPUT_LIMIT*sizeof(char));
    while ( !feof(file_fd) )
    {
      char *ret;
      ret = fgets(buffer, MAX_ARTIST_LEN, file_fd);
      if ( ret == NULL )
        break;
      char *artist = trim(buffer);

      ret = fgets(buffer, MAX_TITLE_LEN, file_fd);
      if ( ret == NULL )
        break;
      char *title = trim(buffer);

      ret = fgets(buffer, MAX_TRACK_LIMIT, file_fd);
      if ( ret == NULL )
        break;

      char ntracks = (char) atoi(buffer);

      ret = fgets(buffer, MAX_TIME_LIMIT, file_fd);
      if ( ret == NULL )
        break;
      int time_m = 0;
      int time_s = 0;
      sscanf(buffer,"%d:%d", &time_m, &time_s);

      curr = allocate(sizeof(dbentry));

      curr->artist = artist;
      curr->title = title;
      curr->tracks = ntracks;
      curr->time_m = time_m;
      curr->time_s = time_s;

      curr->title_next = NULL;

      if (first) {
        curr->artist_next = NULL;
        head = curr;
        first = false;
      } else {
        /* prepend to the head of the list */
        curr->artist_next = head;
        head = curr;
      }
    }
    deallocate(buffer);
  }

  fclose(file_fd);
  return head;
}



void deallocate_db( dbentry* db )
{
  dbentry* curr = db;
  dbentry* next = NULL;

  /* Loop through LL deallocating nodes */
  while( curr )
  {
    next = curr->artist_next;

    deallocate(curr->artist);
    deallocate(curr->title);
    deallocate(curr);

    curr = next;
  }
}

int title_comp( const void * elem1, const void * elem2 )
{
  dbentry* const *dbelem1 = elem1;
  dbentry* const *dbelem2 = elem2;
  return strcmp((*dbelem1)->title, (*dbelem2)->title);
}

int artist_comp( const void * elem1, const void * elem2 )
{
  dbentry* const *dbelem1 =  elem1;
  dbentry* const *dbelem2 =  elem2;
  return strcmp((*dbelem1)->artist, (*dbelem2)->artist);
}

int length( dbentry *list ) {
  int len = 0;
  while ( list )
  {
    ++len;
    list = list->artist_next;
  }
  return len;
}

void print_action(dbentry* head, inaction_t action, char* search)
{
  dbentry* curr = head;

  if (action == artists || action == artist_search) {
    printf("%-24s  %s %35s %s\n", "Name of Artist", "CD Title", "Trk", "Time");
  } else {
    printf("%-39s  %s %14s %s\n", "CD Title", "Name of Artist", "Trk", "Time");
  }

  bool artist_match;
  bool title_match;

  printrule( HEADER, action );
  while ( NULL != curr )
  {
    if (search == NULL) {
      artist_match = false;
      title_match = false;
    } else {
      artist_match = NULL != strstr(curr->artist, search);
      title_match  = NULL != strstr(curr->title, search);
    }

    if ( action == artists || ((action == artist_search) && artist_match) ) {
      printf("%-25s %-40s %3i %02i:%02i\n",
         curr->artist, curr->title, (int) curr->tracks, (int)curr->time_m, (int)curr->time_s);
      curr = curr->artist_next;
    } else if ( action == titles || ((action == title_search) && title_match)) {
      printf("%-40s %-25s %3i %02i:%02i\n",
          curr->title, curr->artist, (int)curr->tracks, (int)curr->time_m, (int)curr->time_s);
      curr = curr->title_next;
    }

    curr = (action == artists || action == artist_search) ? curr->artist_next : curr->title_next;
  }
}

void printrule(unsigned char n, inaction_t action)
{
  if (n == HEADER) {

    if ( action == artists || action == artist_search ) {
      printrule(25,noop);
      printf(" ");
      printrule(40,noop);
    } else {
      printrule(40,noop);
      printf(" ");
      printrule(25,noop);
    }

    printf("%s"," ");
    printrule(3,noop);
    printf("%s"," ");
    printrule(5,noop);
    printf("%s","\n");
    return;
  }

  while (n > 0) {
    printf("%s","="); n--;
  }
}

void sort( dbentry** alist, dbentry** tlist )
{
  dbentry* curr = *alist;
  int len = length( curr );
  dbentry** dbItems = allocate(len * sizeof(dbentry*));
  for( int i = 0; NULL != curr; i++ ) {
    dbItems[i] = curr;
    curr = curr->artist_next;
  }

  // sort items by artist name
  qsort(dbItems, len, sizeof(dbentry*), artist_comp);

  *alist = dbItems[0];

  // reconstruct artist links from newly sorted values
  curr = (dbentry*) dbItems[0];
  for( int i = 1; i < len; i++ ) {
    curr->artist_next = dbItems[i];
    curr = curr->artist_next;
    curr->artist_next = NULL;
  }

  // now sort
  qsort(dbItems, len, sizeof(dbentry*), title_comp);

  *tlist = dbItems[0];

  // reconstruct title links from newly sorted values
  curr = (dbentry*) dbItems[0];
  for( int i = 1; i < len; i++ ) {
    curr->title_next = dbItems[i];
    curr = curr->title_next;
    curr->title_next = NULL;
  }

  deallocate(dbItems);

  return;
}
