/* 
 * Dealer module
 * Simulates the dealer in a blackjack game, interacting with players via servers and ports, sending String messages back and forth
 * that indicate action items and decisions made.
 * Team 15
 * CS50, Fall 2022
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>   
#include <unistd.h> 
#include <sys/socket.h>
#include "../common/card.h"
#include "../common/deck.h"
#include "../common/dealer_hand.h"
#include "../common/network.h"


/********** local function declarations **********/
static bool session_run(int comm_socket);
static void check_send_result(int comm_socket, int player_score, int dealer_score);
static void dealer_response(int comm_socket, deck_t** dealer_deck, hand_t** player_hand, hand_t** dealer_hand, int* player_score, int* dealer_score);
static card_t* deal_card_to_hand(deck_t* deck, hand_t* hand);
static bool dealer_take_card_decision(int dealer_score);
static bool parse_args(int argc, char* argv[], int* num_games, int* port);


/********** main **********/
int main(int argc, char* argv[]){

  int num_games = 0;
  int port = 0;
  
  if(!parse_args(argc, argv, &num_games, &port)){//parse the arguments
    exit(1); //return error 
  }  

  int dealer_socket = start_session(port);
  if(dealer_socket < 0) exit(2); //exit non-zero, defensive programming

  int comm_socket = accept_player(dealer_socket);
  if(comm_socket < 0) exit(5);


  char* player_name = NULL;
  if (comm_socket > 0) {
    while (!recognize_join(comm_socket, &player_name)) {
    }  // wait for player to join
    printf("Player: joined successfully\n");  // print player name
    
    bool in_session = true;
    int games_played = 0;
    while(in_session){ 
     // printf("Starting game %d with player %s\n", games_played, player_name);
      start_game(comm_socket);  // send 'BEGIN'

      if(!session_run(comm_socket)) exit(2); //simulates a single session run

      games_played++; //increment games played
      if (games_played == num_games) {
        in_session = false; //end the outer while loop by converting boolean variable to false
        end_game(comm_socket);  // send 'QUIT'
      }
    }
  }

  close(comm_socket); //close the listening_socket
  shutdown(dealer_socket, SHUT_RDWR); //shut down the dealer_socket
  close(dealer_socket);
  return 0;
}



/********** local functions **********/
/*
* simulates a single session, and will return true or false whether or not another game afterwards will be played or not
*/
static bool session_run(int comm_socket){
  deck_t* dealer_deck = deck_new();

  //srand(time(NULL));
  deck_shuffle(dealer_deck);
  deck_shuffle(dealer_deck); //shuffle two times for insurance
  printf("Deck shuffled\n");

    // Initialize hands
  printf("Creating new hand\n");
  hand_t* player_hand = dealer_hand_new();
  printf("    hand created\n");
  hand_t* dealer_hand = dealer_hand_new();

  //defensive programming, if program out of memory return false, which will exit non-zero in main
  if(player_hand == NULL || dealer_hand == NULL){
    return false; 
  }

  //initial communication as per the lab documentation

  // deal 2 cards to player
  net_deal_card(comm_socket, 0, deal_card_to_hand(dealer_deck, player_hand));
  net_deal_card(comm_socket, 0, deal_card_to_hand(dealer_deck, player_hand));
  // deal 1 card to dealer 
  net_deal_card(comm_socket, 1, deal_card_to_hand(dealer_deck, dealer_hand));

  printf("Cards dealt\n");
  // deck_print(dealer_deck);
  
  int player_score = 0;
  int dealer_score = 0;

  //should continuously run until awaiting_player becomes false
  dealer_response(comm_socket, &dealer_deck, &player_hand, &dealer_hand, &player_score, &dealer_score); 

  check_send_result(comm_socket, player_score, dealer_score);

  deck_delete(dealer_deck);
  dealer_hand_delete(player_hand);
  dealer_hand_delete(dealer_hand);

  return true;
}

/*
* check player score and then send message to player accordingly
*/
static void check_send_result(int comm_socket, int player_score, int dealer_score){
  //send result (1 = win, -1 = lose, 0 = tie)
  if (player_score > 21) { //players loses if player busts
    printf("Player bust\n");
    send_result(comm_socket, -2);
  }else if(dealer_score > 21){ //if dealer busts and player does not player wins
    printf("Player wins\n");
    send_result(comm_socket, 1);
  } else if (player_score == dealer_score) {
    printf("Tie\n");
    send_result(comm_socket, 0);
  } else if (player_score > dealer_score) { //player wins if higher score than dealer and both no bust
    printf("Player wins\n");
    send_result(comm_socket, 1);
  } else { 
    printf("Dealer wins\n"); //dealer wins in last scenario where dealeer_score > player_score
    send_result(comm_socket, -1);
  }
} 

/*
* checks hands of both the player and dealer to observe the current status of the game. 
* updates the scores to variables declared in main
*/
void check_hands(hand_t* player_hand, hand_t* dealer_hand, int* player_score, int* dealer_score){
  printf("Current player hand:\n");
  dealer_hand_print(player_hand);

  *player_score = hand_get_value(player_hand);
  printf("Player hand value: %d\n", *player_score);

  printf("Current dealer hand:\n");
  dealer_hand_print(dealer_hand);

  *dealer_score = hand_get_value(dealer_hand);
  printf("Dealer hand value: %d\n", *dealer_score);
}


/*
* simulates dealers response to the player decision that it recieves.
*/
static void dealer_response(int comm_socket, deck_t** dealer_deck, hand_t** player_hand, hand_t** dealer_hand, int* player_score, int* dealer_score){ //pass in double pointers so variables outside functions can store changes
  bool awaiting_player = true;
  while (awaiting_player && *player_score <= 21) { //goes until awaiting player or until the players score goes above maximum of 21
    request_decision(comm_socket);  // request player decision
    int player_decision = receive_decision(comm_socket);

    if (player_decision == 1) {
      printf("Player hit\n");
      net_deal_card(  //net_deal_card(int comm_socket, int is_dealer_card, char *rank, char *suit)
          comm_socket, 0,
          deal_card_to_hand(*dealer_deck, *player_hand));  // deal player card

    } else if (player_decision == 0) {
      printf("Player stand\n");
      awaiting_player = false;
      deal_card_to_hand(*dealer_deck, *dealer_hand); //at this point dealer flips over his card.

      while(dealer_take_card_decision(hand_get_value(*dealer_hand))){ //after a stand by player now dealer goes to work
        deal_card_to_hand(*dealer_deck, *dealer_hand); //continue to deal cards to dealer until score is 17 or more
        dealer_hand_print(*dealer_hand);
      }
    } else {
      printf("Invalid decision\n");
    }
    check_hands(*player_hand, *dealer_hand, player_score, dealer_score); //checks hands after each run
  }
}

/*
* deals a card from the passed in deck to the respective hand.
*/
static card_t* deal_card_to_hand(deck_t* deck, hand_t* hand) {
  card_t* card = deck_deal(deck);
  hand_add_card(hand, card);
  return card;
}

/*
* checks whether or not the  dealer should stand or hit. If his existing cards are over 17 he cannot hit
*/
static bool dealer_take_card_decision(int dealer_score){
  if(dealer_score >= 17){
    return false;
  }else{
    return true;
  }
}

/*
* parses the arguments, making sure that there are a proper number of them and they are of appropriate type and syntax.
* Return true if arguments valid and false if not
*/
static bool parse_args(int argc, char* argv[], int* num_games, int* port){                
  if(argc != 3){
    perror("Invalid number of arguments passed in to dealer\n");
    return false;
  }

  *num_games = atoi(argv[1]);
  *port =  atoi(argv[2]);
 
    // check for valid port number
  if (*port < 1024 || *port > 65535) {
    fprintf(stderr, "Invalid port number\n");
    return false;
  }
  // check for valid number of games
  if (*num_games < 1) {
    fprintf(stderr, "Invalid number of games\n");
    return false;
  }
  return true;
}