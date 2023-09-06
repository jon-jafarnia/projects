# Makefile for 'Indexer' module
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

querier: querier.o $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LLIBS) -o $@

querier.o: ../common/index.h ../common/pagedir.h ../libcs50/counters.h ../libcs50/file.h ../common/word.h ../libcs50/mem.h


.PHONY: all test valgrind clean

test: querier testing.sh
	bash -v testing.sh >& testing.out

valgrind: querier 
	$(VALGRIND) ./querier < testing.sh

clean:
	rm -rf *.dSYM  # MacOS debugger info
	rm -f *~ *.o
	rm -f core

