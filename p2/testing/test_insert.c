/*
** test program for insert()
**
*/
#include <stdio.h>
#include <sys/types.h>

/*
** Constants for list types
*/

#define	ARTIST	0
#define	TITLE	1

/*
** Constants for success/failure
*/

#define	SUCCESS	1
#define	FAILURE	0

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
** Test data
*/

Node root;

Node albums[] = {
	{ 0, 0, "3 Dog Night", "The Best of 3 Dog Night", 4134, 20 },
	{ 0, 0, "Vogues, The", "Greatest Hits", 3087, 18 },
	{ 0, 0, "Cream", "Strange Brew - The Very Best of Cream", 2632, 12 },
	{ 0, 0, "Iron Butterfly", "In-A-Gadda-Da-Vida", 3488, 8 },
	{ 0, 0, "REO Speedwagon", "The Hits", 3642, 14 },
	{ 0, 0, "Loggins & Messina", "Mother Lode", 2665, 10 },
	{ 0, 0, "Styx", "Cornerstone", 2317, 8 },
	{ 0, 0, "Crosby, Stills & Nash", "Crosby, Stills & Nash", 2440, 10 },
	{ 0, 0, "Styx", "Greatest Hits", 4521, 16 }
};

int n_albums = sizeof(albums) / sizeof(Node);

/*
** Prototype for insert() routine
*/

int insert( Node **root, Node *new, int list );

/*
** Support routine for printing a node or a list of nodes
*/

void printnode( Node *node ) {

	printf( "('%s','%s',%d,%d)",
		node->artist, node->title, node->tracks, node->seconds );

}

void printlist( int which, Node *np ) {

	printf( "  %s list", which == ARTIST ? "artist" : "title" );
	if( np == 0 ) {
		printf( " is empty\n" );
		return;
	} else {
		printf( ":\n" );
	}

	while( np != 0 ) {
		putchar( '\t' );
		printnode( np );
		putchar( '\n' );
		np = which == ARTIST ? np->next_artist : np->next_title;
	}

}

/*
** run insert(&rootnew,list) and print a result based on its return value
**
** always use the global root
*/

void test( int expected, Node *new, int list ) {
	int n;

	/* run the test */

	printf( "Inserting " );
	printnode( new );
	printf( " in %s list\n", list == 0 ? "artist" : "title" );

	n = insert( (Node **) &root, new, list );

	/* report the results */

	printf( "  expected %s, got %s\n",
		expected == SUCCESS ? "success" : "failure",
		n == SUCCESS        ? "success" : "failure" );

}

/*
** The test driver
*/

int main( void ) {
	int i;

	/*
	** insert all nodes into the artist list
	**
	** these should all succeed
	*/

	for( i = 0; i < n_albums; ++i ) {
		test( SUCCESS, &albums[i], ARTIST );
	}

	/*
	** attempt to re-insert an existing node
	*/
	test( FAILURE, &albums[0], ARTIST );

	/*
	** print the resulting artist list
	*/

	printf( "\nResult:" );
	printlist( ARTIST, root.next_artist );
	putchar( '\n' );

	/*
	** insert all nodes into the title list
	**
	** these should all succeed
	*/

	for( i = 0; i < n_albums; ++i ) {
		test( SUCCESS, &albums[i], TITLE );
	}

	/*
	** attempt to re-insert an existing node
	*/
	test( FAILURE, &albums[1], TITLE );

	/*
	** print the resulting artist list
	*/

	printf( "\nResult:" );
	printlist( TITLE, root.next_title );
	putchar( '\n' );

	/*
	** expand here with additional tests
	*/

	return( 0 );

}
