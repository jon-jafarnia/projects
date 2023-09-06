# Makefile for 'Crawler' module
#
# Jon Jafarnia, October 2022
# CS 50, Fall 2022

LIBS =
LLIBS = ../libcs50/libcs50.a ../common/common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) $(FLAGS) -I../libcs50 -I../common
CC = gcc
MAKE = make
FLAGS = -DAPPTEST
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

crawler: crawler.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

crawler.o: ../libcs50/file.h ../libcs50/hashtable.h ../common/pagedir.h ../libcs50/webpage.h ../libcs50/bag.h

.PHONY: all test valgrind clean

# expects a file `test.names` to exist; it can contain any text.
test: crawler testing.sh
	bash -v testing.sh >& testing.out

# expects a file `test.names` to exist; it can contain any text.
valgrind: crawler test.names
	$(VALGRIND) ./crawler < testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core


