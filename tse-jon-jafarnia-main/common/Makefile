# Makefile for 'PageDir' module
#
# Jon Jafarnia, October 2022
# CS 50, Fall 2022

OBJS = pagedir.o index.o word.o
LIB = common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST
CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I../libcs50
CC = gcc
MAKE = make

common.a: pagedir.o index.o word.o
	ar cr $(LIB) $(OBJS)

pagedir.o: pagedir.h ../libcs50/file.h ../libcs50/webpage.h
index.o: index.h ../libcs50/file.h ../libcs50/webpage.h ../libcs50/counters.h ../libcs50/set.h ../libcs50/hashtable.h
word.o: word.h

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o

