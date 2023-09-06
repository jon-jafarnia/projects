/*
dealer_hand.h
Team 15
CS50 Fall 2022
*/

#ifndef __HAND_H
#define __HAND_H

#include "card.h"

/**************** hand struct ****************/
typedef struct hand hand_t;

/**************** public functions ****************/
/*
 * returns pointer to hand struct (that contains no cards), must later be free'd
 */
hand_t* dealer_hand_new();

/*
 * gives a card to the player, returns truth value of its success
 */
bool hand_add_card(hand_t* hand, card_t* card);

/*
 * returns the value of the player's cards, 0 if player has no cards
 */
int hand_get_player_value(hand_t* hand);

/*
 * returns the value of the dealer's card, 0 if dealer has no card
 */
int hand_get_dealer_value(hand_t* hand);

/*
 * returns whether the current calculation of the player's cards' values includes a hard ace 
 * a hard ace is an ace whose value is currently 11 so it can later be decreased to 1
 */
bool hand_is_player_using_hard_ace(hand_t* hand);

/*
 * returns the number of cards the player has
 */
int hand_get_number_player_cards(hand_t* hand);

/*
 * removes and deletees the last card in a player's hand
 * used in q.c to simplify the process of updating q
 */
void hand_remove_last_player_card(hand_t* hand);

/*
 * prints the cards and the card values for both the dealer and player
 */
void hand_print(hand_t* hand);

/*
 * frees the memory used by the given hand
 */
void dealer_hand_delete(hand_t* hand);

/*
 * Build and return new hand.
 */
hand_t *hand_new(void);

/*
 * Add a card to the hand.
 */
bool hand_insert(hand_t *hand, card_t *card);

/*
 * Return the number of cards in the hand.
 */
int hand_size(hand_t *hand);

/*
 * Return the value of the hand.
 */
int hand_get_value(hand_t *hand);

/*
 * Print the hand.
 */
void dealer_hand_print(hand_t *hand);

/*
 * Iterate over the cards in the hand.
 */
hand_t *hand_iterate(hand_t *hand, void *arg,
                     void (*itemfunc)(void *arg, const card_t *card));


#endif  // __HAND_H