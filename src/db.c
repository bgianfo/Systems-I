#include "db.h"
#include "str.h"
#include "common.h"
#include <allocate.h>

/**
 * Check any pointer allocation.
 *
 * @param pointer - The pointer to check
 * 
 * @return false if the allocation failed, true if it's ok.
 */
bool checkalloc( void* pointer ) {
  if (pointer == NULL) {
    fputs( ALLOC_ERROR, stderr );
    return false;
  }
  return true;
}

/**
 * Read in the database file given, returning the head of the linked list.
 *
 * @param filename - filename/path to open.
 *
 * @return head element to the database linked list.
 */
dbentry* read_db(char *filename)
{
  FILE *file_fd = fopen( filename, "r" );
  dbentry* head = NULL;
  char *buffer = NULL;
  if (file_fd == NULL) {
    fputs(DB_FILE_ERROR,stderr);
    return NULL;
  } else {
    int count_n = 0;
    bool first = true;
    dbentry* curr = NULL;
    buffer = allocate(DB_INPUT_LIMIT*sizeof(char));

    if ( !checkalloc(buffer) ) {
      fclose(file_fd);
      return NULL;
    }

    while ( !feof(file_fd) )
    {

      if ( count_n == DB_SIZE_LIMIT ) {
      	fputs( DB_TRUNK_ERROR, stderr );
        break;
      }

      char *ret;

      /** Read all fields for this entry fom file  */

      ret = fgets(buffer, DB_INPUT_LIMIT, file_fd);
      if ( ret == NULL )
        break;

      char *artist = trim_limit(buffer,artists);
      if (!checkalloc( artist )) {
	goto allocfail;
      }

      ret = fgets(buffer, DB_INPUT_LIMIT, file_fd);
      if ( ret == NULL )
        break;

      char *title = trim_limit(buffer, titles);
      if ( !checkalloc( title ) ) {
        unallocate( artist );
	goto allocfail;
      }

      ret = fgets(buffer, DB_INPUT_LIMIT, file_fd);
      if ( ret == NULL )
        break;

      char ntracks = (char) atoi(buffer);

      ret = fgets(buffer, DB_INPUT_LIMIT , file_fd);
      if ( ret == NULL )
        break;

      int time_m = 0;
      int time_s = 0;
      sscanf( buffer, "%d:%d", &time_m, &time_s );

      /* make sure this isn't a duplicate */
      if ( searchdb( head, artist, title ) ) {
        fprintf(stderr,"Duplicate entry: '%s' '%s'\n",artist, title);
        unallocate(artist);
        unallocate(title);
        continue;
      }

      /** create ll node to hold this entry */
      curr = allocate(sizeof(dbentry));
      if ( !checkalloc( curr ) ) {
        unallocate( artist );
        unallocate( title );
	goto allocfail;
      }
      curr->artist = artist;
      curr->title = title;
      curr->tracks = ntracks;
      curr->time_m = time_m;
      curr->time_s = time_s;
      curr->title_next = NULL;

      if ( first ) {
        /* Set the next pointer NULL so we know where the ll stops */
        curr->artist_next = NULL;
        head = curr;
        first = false;
      } else {
        /* prepend to the head of the list */
        curr->artist_next = head;
        head = curr;
      }
      count_n++;
    }
    unallocate(buffer);
  }
  fclose( file_fd );
  return head;

allocfail:

  unallocate(buffer);
  deallocate_db( head );
  head = NULL;
  fclose(file_fd);
  return NULL;

}

/**
 * Iterate through the LL deallocating ll nodes.
 *
 * @param db - database linked list to deallocate
 */
void deallocate_db( dbentry* db )
{
  dbentry* curr = db;
  dbentry* next = NULL;

  while ( curr )
  {
    next = curr->artist_next;

    unallocate(curr->artist);
    unallocate(curr->title);
    unallocate(curr);

    curr = next;
  }
}

/**
 * Look for the given artist and title in the database
 *
 * @param db - database linked list to search in
 * @param art - song artist to search
 * @param tit - song title to search
 *
 * @return true if found, else false.
 */
bool searchdb(dbentry* db, char* art, char* tit ) 
{
  while( NULL != db )
  {
    bool art_match = ( strncmp( db->artist, art, max(strlen(art), strlen(db->artist)) ) == 0);
    bool tit_match = ( strncmp( db->title,  tit, max(strlen(tit), strlen(db->title)) ) == 0);

    if (art_match && tit_match) {
      return true;
    }
    db = db->artist_next;
  }
  return false;
}

/** 
 * Title first comparator
 * Node comparator implementation for the cstd qsort(..) algorithm
 * @param left - left element in list.
 * @param right - right element in list.
 *
 * @return 1 on greater, 0 on equal, -1 on less than.
 */ 
int title_comp( const void * left, const void * right )
{
  dbentry* const *lelem = left;
  dbentry* const *relem = right;

  int title_comparison = strcmp((*lelem)->title, (*relem)->title);

  /* If titles are equal, order by artist */
  if ( 0 == title_comparison )
    return strcmp( (*lelem)->artist, (*relem)->artist );

  return title_comparison;
}

/** 
 * Artist first comparator
 * Node comparator implementation for the cstd qsort(..) algorithm
 * @param left - left element in list.
 * @param right - right element in list.
 *
 * @return 1 on greater, 0 on equal, -1 on less than.
 */ 
int artist_comp( const void * left, const void * right )
{
  dbentry* const *lelem = left;
  dbentry* const *relem = right;

  int artist_comparison = strcmp((*lelem)->artist, (*relem)->artist);

  /* If artists are equal, order by title */
  if ( 0 == artist_comparison )
    return strcmp((*lelem)->title, (*relem)->title);

  return artist_comparison ;
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

/** 
 * Print out the database for several different commands 
 *
 * @param head - the head node of the database to print
 * @param action - The application command we ant to display for:
 *                 artists
 *                 titles
 *                 artist searchterm
 *                 title searchterm
 *
 * @param search - optional paramater used as the term to search
 * for when using a search command.
 */
void print_action(dbentry* head, inaction_t action, char* search)
{
  dbentry* curr = head;
  bool artist_match;
  bool title_match;

  printf("\n");
  if (action == artists || action == artist_search) {
    printf("%-24s  %s %35s %s\n", "Name of Artist", "CD Title", "Trk", "Time");
  } else {
    printf("%-39s  %s %14s %s\n", "CD Title", "Name of Artist", "Trk", "Time");
  }

  printrule( HEADER, action );
  while ( curr )
  {
    if ( NULL == search ) {
      artist_match = false;
      title_match = false;
    } else {
      artist_match = (action == artist_search) && (NULL != strstr(curr->artist, search));
      title_match  = (action == title_search)  && (NULL != strstr(curr->title, search));
    }

    if ( action == artists || artist_match ) {

      printf("%-25s %-40s %3i %02i:%02i\n", curr->artist, curr->title, 
          (int) curr->tracks, (int)curr->time_m, (int)curr->time_s);

    } else if ( action == titles || title_match ) {

      printf("%-40s %-25s %3i %02i:%02i\n", curr->title, curr->artist,
          (int)curr->tracks, (int)curr->time_m, (int)curr->time_s);

    }
    curr = (action == artists || action == artist_search) ? curr->artist_next : curr->title_next;
  }
}


/**
 * Print the horizontal rule used in title/artist commands 
 */
void printrule(unsigned char n, inaction_t action)
{
  if (n == HEADER) {

    if ( action == artists || action == artist_search ) {
      printrule( 25, noop );
      printf(" ");
      printrule( 40,noop );
    } else { /* Must be title or title_search */
      printrule( 40, noop );
      printf(" ");
      printrule( 25, noop );
    }

    printf("%s"," ");
    printrule( 3, noop );
    printf("%s"," ");
    printrule( 5, noop );
    printf("%s","\n");
    return;
  }

  while (n > 0) {
    printf("%s","="); n--;
  }
}

/**
 * Sort the list by artist and by title using qsort(...)
 *
 * @param alist - pointer to pointer of the artist head node.
 * @param tlist - pointer to pointer of the title head node.
 */
void sort( dbentry** alist, dbentry** tlist )
{
  dbentry* curr = *alist;

  int dblen = length( curr );
  dbentry** dbItems = allocate(dblen * sizeof(dbentry*));

  if ( !checkalloc( dbItems ) ) {
    *alist = NULL;
    *tlist = NULL;
  }

  // Copy linked list node pointers to db items
  for( int i = 0; NULL != curr; i++, curr = curr->artist_next ) {
    dbItems[i] = curr;
  }

  // Sort db items by artist name
  qsort( dbItems, dblen, sizeof( dbentry* ), artist_comp );

  // reconstruct artist links in newly sorted values
  *alist = dbItems[0];
  curr = dbItems[0]; 
  for( int i = 1; i < dblen; i++ ) {
    curr->artist_next = dbItems[i];
    curr = curr->artist_next;
    curr->artist_next = NULL;
  }

  /* Now sort db items by title name */
  qsort( dbItems, dblen, sizeof( dbentry* ), title_comp );

  // reconstruct title links innewly sorted values
  *tlist = dbItems[0];
  curr = dbItems[0];
  for( int i = 1; i < dblen; i++ ) {
    curr->title_next = dbItems[i];
    curr = curr->title_next;
    curr->title_next = NULL;
  }

  unallocate(dbItems);
  return;
}
