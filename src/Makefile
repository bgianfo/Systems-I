#
# Created by gmakemake (Sparc Jul 27 2005) on Wed Jan  6 14:12:45 2010
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
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
CC =		gcc
CFLAGS =	-ggdb -std=c99 -I$(ALLOCDIR)
#
# If you are using a SPARC-based machine (one of the compute servers
# holly, hilly, or queeg), change "-lallocate" to "-lallocate-s" below
# to get the correct version of the library.
#
LIBFLAGS =	-L$(ALLOCDIR) -lallocate-s
CLIBFLAGS =	$(LIBFLAGS)

########## End of flags from header.mak


CPP_FILES =	
C_FILES =	 db.c project1.c str.c
H_FILES =	 common.h db.h project1.h str.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	 db.o str.o

#
# Main targets
#

all:	  project1

project1:	project1.o $(OBJFILES)
	$(CC) $(CFLAGS) -o project1 project1.o $(OBJFILES) $(CLIBFLAGS)

#
# Dependencies
#

db.o:	 common.h db.h str.h
project1.o:	 common.h db.h project1.h str.h
str.o:	 common.h str.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -r $(OBJFILES) project1.o core 2> /dev/null

realclean:        clean
	/bin/rm -rf   project1
