ALLOCDIR =	/usr/local/pub/wrc/courses/sp1/allocate
P2DIR =		/usr/local/pub/wrc/courses/sp1/project2
CC =		gcc
AS =		gas
CFLAGS =	-ggdb -std=c99
LIBFLAGS =	-L$(P2DIR) -lproject2 -L$(ALLOCDIR) -lallocate
CLIBFLAGS =	$(LIBFLAGS)

OBJECTS =	mknode.o insert.o print.o

.s.o:
		$(AS) --gstabs $< -o $@

project2:	$(OBJECTS)
	$(CC) $(CFLAGS) -o project2 $(OBJECTS) $(CLIBFLAGS)
