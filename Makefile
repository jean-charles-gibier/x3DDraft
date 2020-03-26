CC = g++
CFLAGS = -c
# Version std
# CXXFLAGS = -Wall
# version dbg CFLAGS = -g -c
CXXFLAGS = -g -Wall
# CXXFLAGS = -nostdinc -I/usr/i486-linuxlibc5/include/g++
INCLUDES = -I/usr/include
LDFLAGS = -L/usr/local/lib -L/usr/X11R6/lib
LIBS = -lm -lX11
# LIBS = -lm -lX11 -lefence

TARGET = x3DDraft
SRCS = x3DDraft.cc \
	gxscreen.cc \
	initialize.cc \
	globals.cc \
	element.cc \
	point3d.cc \
	polypnt.cc \
	assembly.cc \
	motion.cc \
	meta.cc \
	x3DDraft.h \
	gxscreen.h \
	classes.h \
	meta.h

OBJS = x3DDraft.o \
	gxscreen.o \
	initialize.o \
	globals.o \
	element.o \
	point3d.o \
	polypnt.o \
	assembly.o \
	motion.o \
	meta.o

ALL = README Imakefile Makefile.std x3DDraft.man $(SRCS)

all:	$(TARGET)

$(TARGET):	$(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

$(OBJS):	$(SRCS)
	$(CC) $(CFLAGS) $? $(CXXFLAGS)

clean:
	rm -f $(OBJS) $(TARGET) core

shar:	$(ALL)
	shar -o $(TARGET).shar $(ALL)

check:

distcheck:
	file $(TARGET)
