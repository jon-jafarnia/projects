/* 
card.c
Team 15
CS50 Fall 2022
*/

#include "card.h"

/********** local function declarations **********/
int get_card_string_value(char* card_string);
char* assign_card_name(int value);
char* get_card_string_from_string(char* card_string);


/**************** card struct ****************/
typedef struct card{
    char* card_string;
    int value;
} card_t;


/**************** public functions ****************/
card_t* card_new(char* card_string) {
	// check that a value can be parsed from card_string
	int value = get_card_string_value(card_string);
	if (value == 0) {
		fprintf(stderr, "Error. Value could not be parsed from card string\n");
		return NULL;
	}
	// malloc and copy for card card_string
	char* card_string_copy = malloc(strlen(card_string) + 1);
	strcpy(card_string_copy, card_string);
	// create and return new card
    card_t* card = malloc(sizeof(card_t));
	card->card_string = card_string_copy;
	card->value = value;
	return card;
}

card_t* card_new_from_suit_and_identifier(char* suit, int identifier){
    //malloc for size of card
    card_t* new_Card = malloc(sizeof(card_t));
    //safety check on new card
    if(new_Card == NULL){
        perror("Program is out of memory, unable to allocate memory for new card struct variable");
        return NULL; 
    }else{
        //check the given identifier and insert its correct value into card
        if (identifier <11 && identifier > 0){
            new_Card->value = identifier;
        }
        else if (identifier == 11 || identifier == 12 || identifier == 0){
            new_Card->value = 10;
        }
        else{
            //error if invalid input
            perror("Invalid card value, please input a value between 0-12");
        }
        //set name string equal to the string associated with the given identifier
        char* name_string = assign_card_name(identifier);
        new_Card->card_string = malloc(strlen(name_string) + strlen(suit) + 1);
        strncpy(new_Card->card_string, name_string, strlen(name_string)+1); //copy over the string so that struct member variable will actually hold values itself
        strcat(new_Card->card_string, suit);
    }
    //return new card
    return new_Card;
}

card_t* card_new_from_rank(char* rank, char* suit){
    // check and get the value from rank
	int value = get_card_string_value(rank);
	if (value == 0) {
		fprintf(stderr, "Error. Value could not be parsed from card string\n");
		return NULL;
	}
    // malloc and copy for card card_string
    card_t* new_Card = malloc(sizeof(card_t));
    //make string in correct form
	char* name_string = get_card_string_from_string(rank);
    //malloc for the card_string
    new_Card->card_string = malloc(strlen(name_string) + strlen(suit) + 1);
    strncpy(new_Card->card_string, name_string, strlen(name_string)+1); //copy over the string so that struct member variable will actually hold values itself
    strcat(new_Card->card_string, suit);
    //set value equal to given value
    new_Card->value = value;
    //return the card
    return new_Card;
}

int get_card_value(card_t* card){
    //safety check on card
    if(card == NULL){
        perror("card_t parameter is null");
        return 0;
    }else{
        //return int value of card
        return card->value;
    }
}

char* get_card_string(card_t* card){
    //safety check on card
    if(card == NULL){
        perror("card_t parameter is null");
        return NULL;
    }else{
        //return card_string of card
        return card->card_string;
    }
}

bool card_delete(card_t* card) {
    //safety check on card
    if (card == NULL){
        return false;
    } else {
        //free string and card
        free(card->card_string);
        free(card);
        return true;
    }
}

int get_card_string_value(char* card_string) {
    //compare strings and return respective values if valid
    int value = 0;
    if (strncmp(card_string, "Ace", 3) == 0)   		value = 1;
    else if (strncmp(card_string, "Two", 3) == 0)	value = 2;
    else if (strncmp(card_string, "Three", 5) == 0)	value = 3;
    else if (strncmp(card_string, "Four", 4) == 0)	value = 4;
    else if (strncmp(card_string, "Five", 4) == 0)	value = 5;
    else if (strncmp(card_string, "Six", 3) == 0)	value = 6;
    else if (strncmp(card_string, "Seven", 5) == 0)	value = 7;
    else if (strncmp(card_string, "Eight", 5) == 0)	value = 8;
    else if (strncmp(card_string, "Nine", 4) == 0)	value = 9;
    else if (strncmp(card_string, "Ten", 3) == 0)	value = 10;
    else if (strncmp(card_string, "Jack", 4) == 0)	value = 10;
    else if (strncmp(card_string, "Queen", 5) == 0)	value = 10;
    else if (strncmp(card_string, "King", 4) == 0)	value = 10;
	return value;
}

char* get_card_string_from_string(char* card_string) {
    char* new_string;
    //compare strings and return correctly formatted string if valid
    if (strncmp(card_string, "Ace", 3) == 0)   		new_string = "Ace of ";
    else if (strncmp(card_string, "Two", 3) == 0)	new_string = "Two of ";
    else if (strncmp(card_string, "Three", 5) == 0)	new_string = "Three of ";
    else if (strncmp(card_string, "Four", 4) == 0)	new_string = "Four of ";
    else if (strncmp(card_string, "Five", 4) == 0)	new_string = "Five of ";
    else if (strncmp(card_string, "Six", 3) == 0)	new_string = "Six of ";
    else if (strncmp(card_string, "Seven", 5) == 0)	new_string = "Seven of ";
    else if (strncmp(card_string, "Eight", 5) == 0)	new_string = "Eight of ";
    else if (strncmp(card_string, "Nine", 4) == 0)	new_string = "Nine of ";
    else if (strncmp(card_string, "Ten", 3) == 0)	new_string = "Ten of ";
    else if (strncmp(card_string, "Jack", 4) == 0)	new_string = "Jack of ";
    else if (strncmp(card_string, "Queen", 5) == 0)	new_string = "Queen of ";
    else if (strncmp(card_string, "King", 4) == 0)	new_string = "King of ";
	return new_string;
}

char* assign_card_name(int value){
    //check given value, which is an identifier, and set it equal to a correctly formatted string.
    if(value == 2){
            return "Two of ";
    }else if(value == 3){
            return "Three of ";
    }else if(value == 4){
        return "Four of ";
    }else if(value == 5){
        return "Five of ";
    }else if(value == 6){
        return "Six of ";
    }else if(value == 7){
        return "Seven of ";
    }else if(value == 8){
        return "Eight of ";
    }else if(value == 9){
        return "Nine of ";
    }else if(value == 10){
        return "Ten of ";
    }else if(value == 11){
        return "Jack of ";
    }else if(value == 12){
        return "Queen of ";
    }else if(value == 0){
        return "King of ";
    }else if(value == 1){
        return "Ace of ";
    }else{
        perror("you inputted an incorrect suit");
        return 0;
    }
}

