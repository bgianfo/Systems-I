ALLOCDIR =	/usr/local/pub/wrc/courses/sp1/allocate
CC =		gcc
CFLAGS =	-ggdb -std=c99 -I$(ALLOCDIR)
#
# If you are using a SPARC-based machine (one of the compute servers
# holly, hilly, or queeg), change "-lallocate" to "-lallocate-s" below
# to get the correct version of the library.
#
LIBFLAGS =	-L$(ALLOCDIR) -lallocate
CLIBFLAGS =	$(LIBFLAGS)
