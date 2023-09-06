/* 
deck.h
Team 15
CS50 Fall 2022
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "card.h"


typedef struct deck deck_t;

/*
 * Build and return new deck.
 */
deck_t* deck_new(void);

/*
 * Shuffle the deck using the Fisher-Yates shuffle algorithm.
 * Remember to seed the random number generator. e.g. srand(time(NULL));
 * https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
 */
void deck_shuffle(deck_t* deck);

/*
 * Return the next card in the deck.
 */
card_t* deck_deal(deck_t* deck);

/*
 * Print the deck.
 */
void deck_print(deck_t* deck);

/*
 * Free the deck and delete all cards within.
 */
bool deck_delete(deck_t* deck);
