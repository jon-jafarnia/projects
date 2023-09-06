/* 
player.c
Team 15
CS50 Fall 2022
*/

#include <netdb.h>  
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>  
#include "network.h"
#include "q.h"
#include "hand.h"
#include "card.h"


/********** local function declarations **********/
static void print_error_and_usage(char* error);


/********** main **********/
int main(int argc, char *argv[]) {
  // parse args
  if (!(argc == 7 || argc == 8)) {
    print_error_and_usage("Requires 6 or 7 arguments after './player15' (load_q_file is optional depending on load_q_on).");
    exit(1);
  }
  // player_name
  char player_name_array[strlen(argv[1]) + 1];
  char* player_name = &player_name_array[0];
  strcpy(player_name, argv[1]);
  // host
  char host_array[strlen(argv[2]) + 1];
  char* host = &host_array[0];
  strcpy(host, argv[2]);
  // port
  int port = atoi(argv[3]);
  if (port < 1 || port > 65535) {
    print_error_and_usage("Invalid port number.");
    exit(2);
  }
  // training_mode_on
  bool training_mode_on;
  if (strcmp(argv[4], "true") == 0) {
    training_mode_on = true;
  }
  else if (strcmp(argv[4], "false") == 0) {
    training_mode_on = false;
  }
  else {
    print_error_and_usage("Invalid boolean for training_mode_on.");
    exit(3);
  }
  // save_q_file
  char save_q_file_array[strlen(argv[5]) + 1];
  char* save_q_file = &save_q_file_array[0];
  strcpy(save_q_file, argv[5]);
  // load_q_on
  bool load_q_on;
  if (strcmp(argv[6], "true") == 0) {
    load_q_on = true;
  }
  else if (strcmp(argv[6], "false") == 0) {
    load_q_on = false;
  }
  else {
    print_error_and_usage("Invalid boolean for load_q_on.");
    exit(4);
  }
  // load_q_file
  int max_file_name_length = 100;
  char load_q_file_array[max_file_name_length];
  char* load_q_file = &load_q_file_array[0];
  if (load_q_on) {
    strcpy(load_q_file, argv[7]);
  }
  // verify logic
  if (!training_mode_on && !load_q_on) {
    print_error_and_usage("In play mode but didn't specify q file to load.");
    exit(5);
  }

  // connect to server and send player name 
  int comm_socket = connect_to_session(host, port);
  if(comm_socket < 0){
    print_error_and_usage("Could not connect to port on host");
    exit(6);
  }
  request_to_join(comm_socket, player_name);
  printf("Joined game\n");

  // set up for q
  char card_string_array[BUFFER_SIZE];
  char* card_string = &card_string_array[0];
  char result_array[BUFFER_SIZE];
  char* result = &result_array[0];
  hand_t* current_hand = hand_new();
  q_t* q;
  if (load_q_on == true) {
    q = q_load(load_q_file);
    if (q == NULL) {
      fprintf(stderr, "Error. Could not load q from file: %s", load_q_file);
      exit(7);
    }
  }
  else {
    q = q_new();
  }

  // loop through results of server messages
  while (true) {
    // get int state of game, update result and card_string
    int state = player_state(comm_socket, &result, &card_string);

    if (state == 0) {
      printf("Beginning new game.\n");
    }

    else if (state == 1) {
      printf("\tReceived player card: %s\n", card_string);
      hand_add_player_card(current_hand, card_new(card_string));
    }

    else if (state == 2) {
      printf("\tReceived dealer card: %s\n", card_string);
      hand_add_dealer_card(current_hand, card_new(card_string));
    }

    else if (state == 3) {
      // automatically send STAND when player has 21
      if (hand_get_player_value(current_hand) == 21) {
        send_message(comm_socket, "STAND");
      }
      else {
        if (training_mode_on) {
          int decision = rand() % 2;
          if (decision == 0) {
            send_message(comm_socket, "STAND");
          }
          else {
            send_message(comm_socket, "HIT");
          }
        }
        else {
          if (q_get_decision(q, current_hand) == 0) {
            send_message(comm_socket, "STAND");
          }
          else {
            send_message(comm_socket, "HIT");
          }
        }
      }
    }

    else if (state == 4) {
      printf("Received result: %s\n", result);
      q_record_hand(q, current_hand, result);
      hand_delete(current_hand);
      current_hand = hand_new();
    }

    else if (state == 5 || state == -1) {
      if (state == 5) {
        printf("Received message: QUIT\n");
      }
      else {
        printf("Received invalid message.\n");
      }
      printf("Quitting.\n");
      q_save(q, save_q_file);
      q_delete(q);
      if(current_hand != NULL){
        hand_delete(current_hand);
      }
      printf("Client shut down.\n");
      break;
    }
  }
  // close connection
  close(comm_socket);
  return 0;
}


/********** local functions **********/
static void print_error_and_usage(char* error) {
  char* usage = "Usage: ./player15 [player_name] [host] [port] [training_mode_on] [save_q_file] [load_q_on] OPTIONAL: [load_q_file]";
  fprintf(stderr, "Error. %s\n%s\n", error, usage);
}
