CC = g++
CFLAGS = -O
#CXXFLAGS = -nostdinc -I/usr/i486-linuxlibc5/include/g++
INCLUDES = -I/usr/include
LDFLAGS = -s -L/usr/local/lib -L/usr/X11R6/lib
LIBS = -lm -lX11
TARGET = x3DDraft
SRCS = x3DDraft.cc \
	gxscreen.cc \
	initialize.cc \
	globals.cc \
	element.cc \
	point3d.cc \
	polypnt.cc \
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
	meta.o

ALL = README Imakefile Makefile.std x3DDraft.man $(SRCS) x3DDraft.h 

all:	$(TARGET) 

$(TARGET):	$(OBJS) $(SRCS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS) $(LIBS)

.cc  .o .h:
	$(CC) $(CFLAGS) $(INCLUDES) -g $(SRCS)

clean:
	rm -f $(OBJS) $(TARGET) core

shar:	$(ALL)
	shar -o $(TARGET).shar $(ALL)
