/* 
dealer_hand.c simulates the components of a hand for the dealer in blackjack, 
incorporating important functions and card_t struct from the card module
Team 15
CS50 Fall 2022
*/

#include "dealer_hand.h"
#include "card.h"

#define max_player_cards 12


/**************** card struct ****************/
typedef struct hand {   
	card_t** card_array;
} hand_t;


/**************** public functions ****************/
/*
* creates a new hand
*/
hand_t* dealer_hand_new() {
    hand_t* hand = malloc(sizeof(hand_t));
	if(hand == NULL){
		perror("program out of memory");
		return NULL;
	}
    hand->card_array = calloc(max_player_cards, sizeof(card_t*));  // calloc so each player card pointer is initialized to NULL
    if(hand->card_array == NULL){
		perror("program out of memory");
		return NULL;
	}
	return hand;
}

/*
* adds a card to the hand up to the max number of cards a player can have, returns true if successful. 
*/
bool hand_add_card(hand_t* hand, card_t* card) {
    // find first emtpy card index
    int i = 0;
    while (hand->card_array[i] != NULL) {
        if (i == max_player_cards - 1) {
            fprintf(stderr, "Error. Tried to add card to full hand.\n");
            return false;
        }
        i++;
    }
    // insert card into hand
    hand->card_array[i] = card;
    return true;
}

/*
* gets the total value of the hand
*/
int hand_get_value(hand_t* hand) {
    int sum = 0;
	bool player_has_ace = false;
    for (int i = 0; i < max_player_cards && hand->card_array[i] != NULL; i++) {
		sum += get_card_value(hand->card_array[i]);
		if (get_card_value(hand->card_array[i]) == 1) {
			player_has_ace = true;
		}
	}
	if (player_has_ace && sum <= 11) {
		sum += 10;
	}
	return sum;
}

/*
* checks if the player/dealer should count an ace as 1 or as 11 based on the current state of his cards
*/
bool hand_is_using_hard_ace(hand_t* hand) {
	int sum = 0;
	bool player_has_ace = false;
    for (int i = 0; i < max_player_cards && hand->card_array[i] != NULL; i++) {
		sum += get_card_value(hand->card_array[i]);
		if (get_card_value(hand->card_array[i]) == 1) {
			player_has_ace = true;
		}
	}
	if (player_has_ace && sum <= 11) {
		return true;
	}
	return false;
}

/*
* gets the number of cards in the hand
*/
int hand_get_number_cards(hand_t* hand) {
	int i = 0;
	for (; i < max_player_cards && hand->card_array[i] != NULL; i++) {}
	return i;
}

/*
* removes the last card from the hand
*/
void hand_remove_last_card(hand_t* hand) {
	for (int i = max_player_cards - 1; i >= 0; i--) {
		if (hand->card_array[i] != NULL) {
			card_delete(hand->card_array[i]);
			hand->card_array[i] = NULL;
			return;
		}
	}
}

/*
* prints out the contents of the hand
*/
void dealer_hand_print(hand_t* hand) {
	if (hand == NULL) {
		fprintf(stderr, "Error. Hand parameter is NULL.\n");
		return;
	}

	printf("(");
	// player info
	for (int i = 0; i < max_player_cards && hand->card_array[i] != NULL; i++) {
		if (i != 0) {
			printf(", ");
		}
		printf("%s", get_card_string(hand->card_array[i]));
	}
	if (hand->card_array[0] == NULL) {
		printf("no cards");
	}
	printf(")\n");

	// dealer info

}

/*
* frees the actual array pointer and the hand_t
*/
void dealer_hand_delete(hand_t* hand) {
	free(hand->card_array);
	// free hand
	free(hand);
}

