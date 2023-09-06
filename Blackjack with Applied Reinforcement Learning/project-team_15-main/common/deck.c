/* 
deck.h
Team 15
CS50 Fall 2022
*/

#include "deck.h"

#define DECK_SIZE 52
#define NUM_SUITS 4
#define NUM_RANKS 13

typedef struct deck {
  card_t* cards[DECK_SIZE]; //array to hold cards
  bool in_deck[DECK_SIZE]; //array of booleans that is used to help deal the cards
} deck_t;

/*
 * Build and return new deck.
 */
deck_t* deck_new(void) {
  //malloc for deck
  deck_t* new_deck = malloc(sizeof(deck_t));
  //safety check on deck
  if (new_deck == NULL) {
    perror("Unable to allocate memory for new deck struct variable");
    return NULL;
  } else {
    for (int i = 0; i < DECK_SIZE; i++) {
      new_deck->in_deck[i] = true;
    }
    char* suit;
    //Loops for all 52 cards
    for (int i = 0; i < NUM_SUITS; i++) {
      for (int j = 0; j < NUM_RANKS; j++) {
        //Let i correspond to a specific suit
        if (i == 0){
            suit = "Spades";
        }
        if (i == 1){
            suit = "Hearts";
        }
        if (i == 2){
            suit = "Diamonds";
        }
        if (i == 3){
            suit = "Clubs";
        }
        //create new card and indert it into deck
        new_deck->cards[i * NUM_RANKS + j] = card_new_from_suit_and_identifier(suit, j);
      }
    }
  }
  //return the newly built deck
  return new_deck;
}

/*
 * Shuffle the deck using the Fisher-Yates shuffle algorithm.
 * Remember to seed the random number generator. e.g. srand(time(NULL));
 * https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
 */
void deck_shuffle(deck_t* deck) {
  if (deck == NULL) {
    perror("Deck is NULL");
  } else {
    //reset so all cards are in deck
    for (int i = 0; i < DECK_SIZE; i++) {
      deck->in_deck[i] = true;
    }
    // Fisher-Yates shuffle algorithm
    for (int i = DECK_SIZE - 1; i > 0; i--) {
      int j = rand() % (i + 1);
      card_t* temp = deck->cards[i];
      bool temp_in_deck = deck->in_deck[i];
      deck->cards[i] = deck->cards[j];
      deck->in_deck[i] = deck->in_deck[j];
      deck->cards[j] = temp;
      deck->in_deck[j] = temp_in_deck;
    }
  }
}

/*
 * Return the next card in the deck.
 */
card_t* deck_deal(deck_t* deck) {
  //safety check on deck
  if (deck == NULL) {
    perror("Deck is NULL");
    return NULL;
  } else {
    //deal the next card up
    for (int i = 0; i < DECK_SIZE; i++) {
      if (deck->in_deck[i] == true) {
        deck->in_deck[i] = false;
        //return the card
        return deck->cards[i];
      }
    }
  }
  //return null if card not found or error
  return NULL;
}

/*
 * Print the deck.
 */
void deck_print(deck_t* deck) {
  if (deck == NULL) {
    perror("Deck is NULL");
  } else {
    for (int i = 0; i < DECK_SIZE; i++) {
      if (deck->in_deck[i] == true) {
        fprintf(stdout, "card[%02i] ", i);
        // card_print(deck->cards[i]);        not declared in this code
      }
    }
  }
}

/*
 * Free the deck.
 */
bool deck_delete(deck_t* deck) {
  //safety check on deck
  if (deck == NULL) {
    perror("Freeing deck... Deck is NULL");
    return false;
  }
  //delete each card in memory
  for (int i = 0; i < DECK_SIZE; i++) {
    card_delete(deck->cards[i]);
  }
  //free the deck and return true if worked
  free(deck);
  return true;
}