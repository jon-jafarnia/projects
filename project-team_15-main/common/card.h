/* 
card.h
Team 15
CS50 Fall 2022
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef CARD_H
#define CARD_H


typedef struct card card_t;
/*
* build and return new card when given a string such as "Ace of Diamonds"
* The user must later free this memory using card_delete
*/
card_t* card_new(char* card_string);

/*
* Build and return new card when given a suit (i.e. Spades, Clubs, Hearts, Diamonds), and an identifier (int 0-12)
* The user must later free this memory using card_delete
*/
card_t* card_new_from_suit_and_identifier(char* suit, int identifier);

/*
* build and return new card based off of the rank (Ace, Two, Jack, etc.) and suit (i.e. Spades, Clubs, Hearts, Diamonds)
*/
card_t* card_new_from_rank(char* rank, char* suit);

/*
* takes a card as a paramter and returns the int value of that card
*/
int get_card_value(card_t* card);

/*
* takes a card as a paramter and returns the char* card_string of the card
*/
char* get_card_string(card_t* card);

/*
* Deletes the card and frees up its memory and its char* member variable
* We guarantee this works with no leaks
*/
bool card_delete(card_t* card);

/*
* based on the passed in char* card_string (i.e. Ace, Two, Jack, etc.) return that strings respective int value
* 1-10: 1-10
* J,Q,K: 10
* A: 1
*/
int get_card_string_value(char* card_string);

/*
* based on the passed in char* card_string return the respective char* value in correct form
* This function essentially normalizes the message to fit the proper format
* 1-10: One of, Two of, etc.
* J,Q,K: Jack of, Queen of, King of
* A: Ace of
*/
char* get_card_string_from_string(char* card_string);

/*
* based on the passed in int value return the respective char* card_string
* 1-10: One of, Two of, Three of, Four of...
* 11, 12, 13: Jack of, Queen of, King of
* 14: Ace of
*/
char* assign_card_name(int value);

#endif  // CARD_H