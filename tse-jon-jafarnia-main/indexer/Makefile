# Makefile for 'Crawler' module
#
# Jon Jafarnia, October 2022
# CS 50, Fall 2022

LLIBS = ../libcs50/libcs50-given.a ../common/common.a

# uncomment the following to turn on verbose memory logging
#TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) $(FLAGS) -I../libcs50 -I../common
CC = gcc
MAKE = make
FLAGS = -DAPPTEST
# for memory-leak tests
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all

indexer: indexer.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

indextest: indextest.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

indexer.o: ../libcs50/file.h ../libcs50/hashtable.h ../common/pagedir.h ../libcs50/webpage.h ../common/index.h 

indextest.o: indexer.o ../libcs50/file.h ../libcs50/hashtable.h ../common/pagedir.h ../libcs50/webpage.h ../libcs50/bag.h ../common/index.h


.PHONY: all test valgrind clean

# expects a file `test.names` to exist; it can contain any text.
test: indexer testing.sh
	bash -v testing.sh >& testing.out

# expects a file `test.names` to exist; it can contain any text.
valgrind: indexer test.names
	$(VALGRIND) ./indexer < testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core



