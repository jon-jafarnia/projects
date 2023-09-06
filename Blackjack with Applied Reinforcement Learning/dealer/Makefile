# Makefile for the dealer module of the Blackjack game
# CS50 Fall 2022 - team 15

C = ../common
LLIBS = $C/common.a
CC = gcc
VALGRIND = valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes 

CFLAGS = -Wall -pedantic -ggdb -std=c11 -I$C

# Arguments
# usage: ./dealer [num_games] [num_players] [port]
NUM_GAMES = 10
NUM_PLAYERS = 1
PORT = 8095

.PHONY: all clean test

all: dealer15

dealer15: dealer.o $(LLIBS)
	$(CC) $(CFLAGS) $(LLIBS) $(FLAGS) $^  -o $@

dealer.o: $C/network.h $C/deck.h $C/card.h $C/dealer_hand.h

#Test with testing.sh
test: dealer15 testing.sh
	bash -v testing.sh &> testing.out


#Testing with valgrind
valgrind: dealer15
	$(VALGRIND) ./dealer15 $(NUM_GAMES) $(PORT) 

clean:
	rm -f *~ *.o *.out *.log *.dSYM
	rm -f dealer15
	rm -f testing.out