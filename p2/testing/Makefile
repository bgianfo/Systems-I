#
# Makefile for individual testing of project 2 functions
#
ALLOCDIR =	/usr/local/pub/wrc/courses/sp1/allocate
CC =		gcc
AS =		gas
CFLAGS =	-ggdb -std=c99 -I$(ALLOCDIR)
LIBFLAGS =	-L$(ALLOCDIR) -lallocate
CLIBFLAGS =	$(LIBFLAGS)

.s.o:
		$(AS) --gstabs $< -o $@

#
# Test programs for SP1 project 2 functions
#

all:	test_mknode test_insert test_print

run:	all
	./test_mknode

test_mknode:	test_mknode.o mknode.o
	$(CC) $(CFLAGS) -o test_mknode test_mknode.o mknode.o $(LIBFLAGS)

test_insert:	test_insert.o insert.o
	$(CC) $(CFLAGS) -o test_insert test_insert.o insert.o $(LIBFLAGS)

test_print:	test_print.o print.o
	$(CC) $(CFLAGS) -o test_print test_print.o print.o $(LIBFLAGS)

clean:
	rm -f *.o test_mknode test_insert test_print core
