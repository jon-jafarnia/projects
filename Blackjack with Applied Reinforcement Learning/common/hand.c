/* 
hand.c
Team 15
CS50 Fall 2022
*/

#include "hand.h"

#define max_player_cards 12


/**************** card struct ****************/
typedef struct hand {   //WHY WHOULD YOU DO IT THIS WAY
	card_t* dealer_card;
	card_t** player_cards;
} hand_t;


/**************** public functions ****************/
hand_t* hand_new() {
    hand_t* hand = malloc(sizeof(hand_t));
    hand->dealer_card = NULL;
    hand->player_cards = calloc(max_player_cards, sizeof(card_t*));  // calloc so each player card pointer is initialized to NULL
    return hand;
}

bool hand_add_player_card(hand_t* hand, card_t* card) {
    // find first emtpy card index
    int i = 0;
    while (hand->player_cards[i] != NULL) {
        if (i == max_player_cards - 1) {
            fprintf(stderr, "Error. Tried to add card to full hand.\n");
            return false;
        }
        i++;
    }
    // insert card into hand
    hand->player_cards[i] = card;
    return true;
}

bool hand_add_dealer_card(hand_t* hand, card_t* card) {
    if (hand->dealer_card != NULL) {
		fprintf(stderr, "Error. Dealer card already set.\n");
		return false;
	}
	hand->dealer_card = card;
	return true;
}

int hand_get_player_value(hand_t* hand) {
    int sum = 0;
	bool player_has_ace = false;
    for (int i = 0; i < max_player_cards && hand->player_cards[i] != NULL; i++) {
		sum += get_card_value(hand->player_cards[i]);
		if (get_card_value(hand->player_cards[i]) == 1) {
			player_has_ace = true;
		}
	}
	if (player_has_ace && sum <= 11) {
		sum += 10;
	}
	return sum;
}

int hand_get_dealer_value(hand_t* hand) {
	if (hand->dealer_card == NULL) {
		return 0;
	}
	int dealer_card_value = get_card_value(hand->dealer_card);
	if (dealer_card_value == 1) {
		dealer_card_value = 11;
	}
	return dealer_card_value;
}

bool hand_is_player_using_hard_ace(hand_t* hand) {
	int sum = 0;
	bool player_has_ace = false;
    for (int i = 0; i < max_player_cards && hand->player_cards[i] != NULL; i++) {
		sum += get_card_value(hand->player_cards[i]);
		if (get_card_value(hand->player_cards[i]) == 1) {
			player_has_ace = true;
		}
	}
	if (player_has_ace && sum <= 11) {
		return true;
	}
	return false;
}

int hand_get_number_player_cards(hand_t* hand) {
	int i = 0;
	for (; i < max_player_cards && hand->player_cards[i] != NULL; i++) {}
	return i;
}

void hand_remove_last_player_card(hand_t* hand) {
	for (int i = max_player_cards - 1; i >= 0; i--) {
		if (hand->player_cards[i] != NULL) {
			card_delete(hand->player_cards[i]);
			hand->player_cards[i] = NULL;
			return;
		}
	}
}

void hand_print(hand_t* hand) {
	if (hand == NULL) {
		fprintf(stderr, "Error. Hand parameter is NULL.\n");
		return;
	}

	// player info
	printf("Player: %2d (", hand_get_player_value(hand));
	for (int i = 0; i < max_player_cards && hand->player_cards[i] != NULL; i++) {
		if (i != 0) {
			printf(", ");
		}
		printf("%s", get_card_string(hand->player_cards[i]));
	}
	if (hand->player_cards[0] == NULL) {
		printf("no cards");
	}
	printf(")\n");

	// dealer info
	printf("Dealer: %2d ", hand_get_dealer_value(hand));
	if (hand->dealer_card == NULL) {
		printf("(no card)\n");
	}
	else {
		printf("(%s)\n", get_card_string(hand->dealer_card));
	}
}

void hand_delete(hand_t* hand) {
	// free dealer card
	card_delete(hand->dealer_card);
	// free player_cards
	int i = 0;
	while (i < max_player_cards && hand->player_cards[i] != NULL) {
		card_delete(hand->player_cards[i]);
		i++;
	}
	card_delete(hand->player_cards[i]);
	free(hand->player_cards);
	free(hand);
}


