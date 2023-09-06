# Makefile for the Blackjack game
# cs50 Fall 2022 - team 15


.PHONY: all common player dealer clean

all: common player dealer

common: 
	make -C common
player:
	make -C player
dealer:
	make -C dealer

clean:
	rm -f *~
	make -C common clean
	make -C dealer clean
	make -C player clean
