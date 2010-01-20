#
# Created by gmakemake (Sparc Aug  2 2007) on Mon Jan 18 00:48:52 2010
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c

########## Flags from header.mak

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

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	
S_FILES =	insert.s mknode.s print.s
H_FILES =	
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	insert.o mknode.o print.o

#
# Main targets
#

all:	

#
# Dependencies
#


#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm $(OBJFILES) core 2> /dev/null

realclean:        clean
	-/bin/rm -rf 
