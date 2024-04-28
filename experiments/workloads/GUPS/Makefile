HEMEMDIR = $(CURDIR)/lib/
TIMER = ./lib/timer.o
CC = gcc
CFLAGS = -g -Wall -O3
INCLUDES = -I../linux/usr/include
LIBS = -lm -lpthread

default: gups-pebs gups-random gups-hotset-move

all: gups-pebs gups-random gups-simple gups-lru gups-lru-swap gups-hotset-move #gups

gups-random: gups-random.o $(TIMER)
	$(CC) $(CFLAGS) $(INCLUDES) -o gups-random gups-random.o zipf.o $(TIMER) $(LIBS) # -L$(HEMEMDIR) 

gups-small: gups-small.o $(TIMER)
	$(CC) $(CFLAGS) $(INCLUDES) -o gups-small gups-small.o $(TIMER) $(LIBS)

gups: gups.o $(TIMER)
	$(CC) $(CFLAGS) $(INCLUDES) -o gups gups.o zipf.o $(TIMER) $(LIBS)

gups-pebs: gups.o 
	$(CC) $(CFLAGS) $(INCLUDES) -o gups-pebs gups.o zipf.o $(TIMER) $(LIBS)

gups-hotset-move: gups-hotset-move.o
	$(CC) $(CFLAGS) $(INCLUDES) -o gups-hotset-move gups-hotset-move.o zipf.o $(TIMER) $(LIBS)

gups-simple: gups.o
	$(CC) $(CFLAGS) $(INCLUDES) -o gups-simple gups.o zipf.o $(LIBS) -L$(HEMEMDIR) -simple

gups-lru: gups.o
	$(CC) $(CFLAGS) $(INCLUDES) -o gups-lru gups.o zipf.o $(LIBS) -L$(HEMEMDIR) -lru

gups-lru-swap: gups.o
	$(CC) $(CFLAGS) $(INCLUDES) -o gups-lru-swap gups.o zipf.o $(LIBS) -L$(HEMEMDIR) -lru-swap

gups-random.o: gups-random.c zipf.c gups.h
	$(CC) $(CFLAGS) $(INCLUDES) -c gups-random.c zipf.c

gups.o: gups.c zipf.c  gups.h
	$(CC) $(CFLAGS) $(INCLUDES) -c gups.c zipf.c

gups-hotset-move.o: gups-hotset-move.c zipf.c gups.h
	$(CC) $(CFLAGS) $(INCLUDES) -c gups-hotset-move.c zipf.c

./lib/timer.o: ./lib/timer.c ./lib/timer.h
	$(CC) $(CFLAGS) $(INCLUDES) -c ./lib/timer.c

clean:
	$(RM) *.o gups gups-hotset-move gups-lru-swap gups-lru gups-simple gups-random gups-pebs
