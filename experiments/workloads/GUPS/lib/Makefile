CC = gcc
CFLAGS = -g -Wall -O3 -fPIC
INCLUDES = -I../linux/usr/include/
LIBS = -lm -lpthread

default: timer.o

all: timer.o

timer.o: timer.c timer.h
	$(CC) $(CFLAGS) $(INCLUDES) -c timer.c

clean:
	$(RM) *.o *.so
