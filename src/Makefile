CC =		gcc
CFLAGS =	-ggdb -std=c99 -Wall -Wextra -pedantic
#
# If you are using a SPARC-based machine (one of the compute servers
# holly, hilly, or queeg), change "-lallocate" to "-lallocate-s" below
# to get the correct version of the library.
#
# LIBFLAGS =	-L$(ALLOCDIR) -lallocate
# CLIBFLAGS =	$(LIBFLAGS)

default: project1.c
	$(CC) $(CFLAGS) project1.c -o project1

clean:
	rm -rf *.o
