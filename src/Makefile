CC = colorgcc
CFLAGS = --std=c99 -Wall -Wextra -pedantic -fno-strict-aliasing  \
		-Wchar-subscripts -Wundef -Wshadow -Wcast-align -Wwrite-strings \
    -Wsign-compare -Wunused -Wno-unused-parameter -Winit-self \
    -Wpointer-arith -Wredundant-decls -Wformat-nonliteral \
    -Wno-format-zero-length -Wmissing-format-attribute -Wmissing-prototypes \
    -Wstrict-prototypes
#
# If you are using a SPARC-based machine (one of the compute servers
# holly, hilly, or queeg), change "-lallocate" to "-lallocate-s" below
# to get the correct version of the library.
#
# LIBFLAGS =	-L$(ALLOCDIR) -lallocate
# CLIBFLAGS =	$(LIBFLAGS)

default: clean project1.c
	touch allocate.h
	$(CC) $(CFLAGS) db.c -c 
	$(CC) $(CFLAGS) project1.c -c 
	$(CC) $(CFLAGS) str.c -c 
	$(CC) $(CFLAGS) str.o project1.o db.o -o project1
	rm allocate.h

clean:
	rm -rf *.o
	rm -rf project1
	rm -rf allocate.h
