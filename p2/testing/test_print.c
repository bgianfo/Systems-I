/*
** test program for print()
**
*/
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>

/*
** Constants for list types
*/

#define	ARTIST	0
#define	TITLE	1
#define	ARTISTS	4
#define	TITLES	5

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
**
** the extern declaration is needed to keep the compiler happy
*/

extern Node tdn, cre, csn, iro, lnm, reo, st1, st2, vog;

Node root = {
	&tdn,	/* first node in artist list:  "3 Dog Night" */
	&st1	/* first node in title list:   "Cornerstone" */
};

/*
** Albums
**
** Artist order:
**	3 Dog Night
**	Cream
**	Crosby, Stills & Nash
**	Iron Butterfly
**	Loggins & Messina
**	REO Speedwagon
**	Styx (Cornerstone)
**	Styx (Greatest Hits)
**	Vogues, The
**
** Title order:
**	Cornerstone
**	Crosby, Stills & Nash
**	Greatest Hits (Styx)
**	Greatest Hits (Styx)
**	In-A-Gadda-Da-Vida
**	Mother Lode
**	Strange Brew - The Very Best of Cream
**	The Best of 3 Dog Night
**	The Hits
*/

Node tdn = {
	&cre, &reo, "3 Dog Night", "The Best of 3 Dog Night", 4134, 20
};

Node cre = {
	&csn, &tdn, "Cream", "Strange Brew - The Very Best of Cream", 2632, 12
};

Node csn = {
	&iro, &st2, "Crosby, Stills & Nash", "Crosby, Stills & Nash", 2440, 10
};

Node iro = {
	&lnm, &lnm, "Iron Butterfly", "In-A-Gadda-Da-Vida", 3488, 8
};

Node lnm = {
	&reo, &cre, "Loggins & Messina", "Mother Lode", 2665, 10
};

Node reo = {
	&st1,   0, "REO Speedwagon", "The Hits", 3642, 14
};

Node st1 = {
	&st2, &csn, "Styx", "Cornerstone", 2317, 8
};

Node st2 = {
	&vog, &vog, "Styx", "Greatest Hits", 4521, 16
};

Node vog = {
	  0, &iro, "Vogues, The", "Greatest Hits", 3087, 18
};

/*
** Prototype for print() routine
*/

void print( Node **root, int list, char *target ); /* {
	Node *node = *root;
	node++;
	node--;
	Node *node2 = node->next_artist;

  printf("%-40s %-25s %3i %02i:%02i\n", node2->title, node2->artist, node2->tracks, 0,0); 

}
*/

/*
** The test driver
*/

int main( void ) {
	int i;

	/*
	** print the full lists
	*/

	print( &root.next_artist, ARTISTS, 0 );
	putchar( '\n' );
	print( &root.next_artist, TITLES, 0 );
	putchar( '\n' );

	/*
	** print just the Styx entries
	*/

	print( &root.next_artist, ARTIST, "Styx" );
	putchar( '\n' );

	/*
	** print just the "Greatest Hits" entries
	*/

	print( &root.next_artist, TITLE, "Greatest Hits" );

	/*
	** expand here with additional tests
	*/

	return( 0 );

}
