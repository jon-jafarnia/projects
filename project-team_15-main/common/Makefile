# Makefile - common library
# Team 15
# CS50 Fall 2022

LIB = common.a
OBJS = card.o deck.o network.o q.o dealer_hand.o hand.o

CFLAGS = -Wall -pedantic -std=c11 -ggdb

$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

card.o: card.h
deck.o: deck.h card.h
hand.o: hand.h card.h
network.o: network.h card.h 
q.o: q.h hand.h card.h
dealer_hand.o: dealer_hand.h card.h

clean:
	rm -f *~ 
	rm -f *.o 
	rm -f *.dSYM
	rm -f $(LIB)