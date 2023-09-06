# Makefile for the player module of the Blackjack game
# CS50 Fall 2022 - team 15

C = ../common
LLIBS = $C/common.a
CC = gcc
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all
CFLAGS = -Wall -pedantic -ggdb -std=c11 $(FLAGS) -I$C

.PHONY: all clean test

all: player15

player15: player.o $(LLIBS)
	$(CC) $(CFLAGS) $(FLAGS) $^ -o $@

player.o: $C/network.h $C/deck.h $C/card.h $C/hand.h

test: 
	bash -v testing.sh &> testing.out

clean:
	rm -f *~ *.o *.out *.log *.dSYM
	rm -f player15
	rm -f testing.out