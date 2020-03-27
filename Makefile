CC = g++

# version dbg 
# CFLAGS = -g -c

CFLAGS = -c
CXXFLAGS = -g -Wall
# Version std
# CXXFLAGS = -Wall
# CXXFLAGS = -nostdinc -I/usr/i486-linuxlibc5/include/g++

LDFLAGS = -L/usr/local/lib -L/usr/X11R6/lib
LIBS = -lm -lX11
# verion check with valgrind
# LIBS = -lm -lX11 -lefence


SRCDIR = src
OBJDIR = obj
INCDIR = include

INCLUDES = -I$(INCDIR)

TARGET = x3DDraft

ADDHEADER= x3DDraft.h \
	gxscreen.h \
	classes.h \
	meta.h


SRCS = $(shell ls $(SRCDIR)/*.cc)
HEADS = $(shell ls $(INCDIR)/*.h)
OBJS = $(patsubst %.cc, $(OBJDIR)/%.o, $(subst $(SRCDIR)/,,$(SRCS)))

ALL = README Imakefile Makefile.std x3DDraft.man $(SRCS)

all:	$(TARGET)

$(TARGET):	$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJS):	$(SRCS)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $(patsubst $(OBJDIR)/%.o, $(SRCDIR)/%.cc, $@) $(CXXFLAGS) 

clean:
	rm -f $(OBJS) $(TARGET) core

shar:	$(ALL)
	shar -o $(TARGET).shar $(ALL)

distcheck:
	file $(TARGET)

check:
	file $(TARGET)

# .PHONY: all
#all: ; $(info $$var is [${var}])echo this
