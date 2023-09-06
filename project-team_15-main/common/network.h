/* 
network.h
Team 15
CS50 Fall 2022
*/

#ifndef NETWORK_H
#define NETWORK_H

#include <stdbool.h>
#include <stdio.h>
#include <netdb.h>  
#include <stdlib.h>
#include <string.h>
#include <strings.h>  
#include <unistd.h>   
#include <time.h>
#include <errno.h>  
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include "card.h"


#define DEFAULT_PLAYER_NAME "team_15"
#define DEFAULT_HOST "localhost"
#define DEFAULT_PORT 8080 + 15
#define BUFFER_SIZE 128

/*
* sends the message passed in via the comm_socket
* Arguments: int comm_socket, char *message
* Returns: void
*/
void send_message(int comm_socket, char *message);
/*
* Receives message from socket, which it copies over to the passed in message
* Arguments: int comm_socket, char *message
* Returns: void
*/
void receive_message(int comm_socket, char *message);

/* Initialize a socket for the server(dealer) and start listening
 * Arguments: none
 * Returns: socket file descriptor
 * We Guarantee: socket file descriptor is valid and listening
 * Caller is responsible for closing the socket
 */
int start_session(int serve_port);

/* Accept a connection from a client(player)
 * Arguments: server socket file descriptor
 * Returns: file stream for the communication socket
 * We Guarantee: file stream is valid and connected
 * Caller is responsible for closing the file stream
 */
int accept_player(int server_socket);

/* Send 'BEGIN' message to the client(player
 * Arguments: open/connected socket file descriptor
 */
void start_game(int comm_socket);

/* Recognize join request
 * Arguments: File descriptor for the socket
 * Returns: true if successful, false otherwise
 * We print message indicating player name
 */
bool recognize_join(int comm_socket, char **player_name);

/* Send message indicating player card revealed by dealer(server)
 * Arguments: File stream for the communication socket, card as a card_t
 * Returns: true if successful, false otherwise
 * Message format: "CARD <rank> of <suit>"
 */
void net_deal_card(int comm_socket, int is_dealer_card, card_t* card);

/* Send the message 'DECISION' to request the player's choice
 * Arguments: File descriptor for the communication socket
 * Returns: none
 * We Guarantee: 'DECISION' message is sent
 * We Assume: connection is established and still open
 */
void request_decision(int comm_socket);

/* Receive hit or stand decision
 * Arguments: File descriptor for the communication socket, a pointer to int
 * Returns: true if a decision 'HIT' or 'STAND' is received, false otherwise
 * We update the int pointed to by the second argument to 1 if 'HIT' is received
 * and 0 if 'STAND' is received
 */
int receive_decision(int comm_socket);

/* Send the results for a game
 * Arguments: File descriptor for the communication socket, result as an int
 *             (-1: loss, 1: win, 0: push)
 * Returns: none
 * We Guarantee: 'RESULT' message is sent
 * We Assume: connection is established and still open
 */
void send_result(int comm_socket, int result);

/* Send the message 'QUIT' to the client
 * This signifies the end of the game
 * Arguments: File descriptor for the communication socket
 * Returns: none
 * We Guarantee: 'QUIT' message is sent
 * We Assume: connection is established and still open
 */
void end_game(int comm_socket);

/* ===========================================================================*/
/* ============================ Player Methods ===============================*/
/* ===========================================================================*/

/* Connect to an open game session
 * Arguments: server hostname, server port
 * Returns: File stream for the socket
 * We Guarantee: File stream is valid
 * Caller is responsible for closing the file stream
 */
int connect_to_session(char *hostname, int port);

/* Send JOIN request
 * Arguments: File stream for the socket, player name
 * Returns: true if successful, false otherwise
 * We Guarantee: 'JOIN' request is sent
 * We assume: connection is already established and still open; player name is
 * valid
 */
void request_to_join(int comm_socket, char *player_name);

/* send hit or stand decision
 * Arguments: File stream for the communication socket, decision as an int
 *            (0: stand, 1: hit)
 * Returns: true if successful, false otherwise
 * We Guarantee: 'HIT' or 'STAND' message is sent
 * We Assume: connection is established and still open
 */
void send_decision(int comm_socket, int decision);

/*
 *  Recieve dealer instructions/commands and returns a cooresponding int value
 *  Also stores the result of the hand (BUST, LOOSE, etc) in result if applicable
 *  And stores the card given during that dealer message in card_string if applicable
 */
int player_state(int comm_socket, char** result, char** card_string);

#endif  // NETWORK_H