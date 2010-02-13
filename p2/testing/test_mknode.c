/*
** test program for mknode()
**
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdint.h>
#include <allocate.h>

/*
** Definition of an item node (based on the project 2 specification)
*/

typedef
	struct node {
		struct node *next_artist;	/* next artist */
		struct node *next_title;	/* next title */
		char        *artist;		/* artist name */
		char        *title;		/* item title */
		uint16_t    seconds;		/* time, in seconds */
		uint8_t     tracks;		/* track count */
		uint8_t     other;		/* other stuff */
	}
		Node;

/*
** Prototype for mknode() routine
*/

Node *mknode( char *title, char *artist, int tracks, int seconds );

/*
** run mknode(title,artist,tracks,seconds) and print a result based on
** its return value
*/

void test( char *title, char *artist, int tracks, int seconds ) {
	Node *np;

	/* run the test */

	printf( "Testing mknode('%s','%s',%d,%d):\n\tallocation:  ",
		title, artist, tracks, seconds );

	np = mknode( title, artist, tracks, seconds );

	/* report the results */

	if( np == 0 ) {

		printf( "failed!\n" );
	
	} else {
		int n = 0;

		printf( "succeeded\n" );
		if( np->next_artist ) {
			printf( "\tnext_artist != 0\n" );
			++n;
		}
		if( np->next_title ) {
			printf( "\tnext_title != 0\n" );
			++n;
		}
		if( np->artist != artist ) {
			printf( "\tartist 0x%08x != expected 0x%08x\n",
				np->artist, artist );
			++n;
		}
		if( np->title != title ) {
			printf( "\ttitle 0x%08x != expected 0x%08x\n",
				np->title, title );
			++n;
		}
		if( np->seconds != seconds ) {
			printf( "\tseconds %d != expected %d\n",
				np->seconds, seconds );
			++n;
		}
		if( np->tracks != tracks ) {
			printf( "\ttracks %d != expected %d\n",
				np->tracks, tracks );
			++n;
		}
		if( n == 0 ) printf( "\tfields correct\n" );
		unallocate( np );
	}

	putchar( '\n' );

}

/*
** The test driver
*/

int main( void ) {
	int n;

	/*
	** test( title, artist, tracks, seconds );
	*/

	test( "REO Speedwagon", "The Hits", 14, 3642 );
	test( "Iron Butterfly", "In-A-Gadda-Da-Vida", 8, 3488 );
	test( "Crosby, Stills & Nash", "Crosby, Stills & Nash", 10, 2440 );
	test( "3 Dog Night", "The Best of 3 Dog Night", 20, 4134 );
	test( "Cream", "Strange Brew - The Very Best of Cream", 12, 2632 );
	test( "Loggins & Messina", "Mother Lode", 10, 2665 );

	/*
	** expand here with additional tests
	*/

	return( 0 );

}
