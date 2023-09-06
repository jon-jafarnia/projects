/*
netword.c
Team 15
CS50 Fall 2022
*/

#include "network.h"

#define LISTEN_BACKLOG 5  // number of connections to keep waiting
#define MSG_SIZE 25       // max size of a message


/********** local function declarations **********/
/*
 * sleeps for 26 milliseconds; must declare nanosleep before
 */
int nanosleep(const struct timespec *req, struct timespec *rem);
static void sleep_26();


/********** public functions **********/
/*
* Helper function that creates teh TCP socket with helper socket() function. 
*/
int tcp_ipv4_socket() {
  int opt = 1;
  int sock = socket(AF_INET, SOCK_STREAM, 0);

  if (sock == -1) {
    perror("ERROR opening server socket");
    return -1;
    
  }
  if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
  }
  fprintf(stdout,"TCP socket created successfully\n");
  return sock;
}

/*
* send the message passed in as an argument to the port at the comm_socket 
*/
void send_message(int comm_socket, char *message) {
  sleep_26();
  char buffer[BUFFER_SIZE];
  bzero(buffer, BUFFER_SIZE);
  strcpy(buffer, message);
  printf("Sending message: %s\n", buffer);
  send(comm_socket, buffer, MSG_SIZE, 0);
  return;
}

/*
* reads a message from the port at comm_socket and have the passed in message pointer argument point to it
*/
void receive_message(int comm_socket, char *message) {
  sleep_26();
  char buffer[BUFFER_SIZE];
  bzero(buffer, BUFFER_SIZE);
  read(comm_socket, buffer, MSG_SIZE);
  strcpy(message, buffer);
  return; 
}

/***************************** Dealer Methods ********************************/

/* Initialize a socket for the server(dealer) and start listening */
int start_session(int serve_port) {
  struct sockaddr_in server_address;

  int server_socket = tcp_ipv4_socket();
  if(server_socket < 0) return -1; //defensive programming
  
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(serve_port);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int n = bind(server_socket, (struct sockaddr *)&server_address,
               sizeof(server_address));
  if (n < 0) { //defensive programming
    perror("ERROR on binding");
    return -1;
  }
  printf("Server socket binded successfully to port: %d\n", serve_port);

  listen(server_socket, LISTEN_BACKLOG);
  printf("Server socket is waiting for player's to connect...\n");

  return server_socket;
}

/* Accept a connection from a client(player) */
int accept_player(int server_socket) {
  struct sockaddr_in addr;
  socklen_t addr_size;
  addr_size = sizeof(addr);

  int comm_socket = accept(server_socket, (struct sockaddr *)&addr, &addr_size);
  if (comm_socket < 0) { //defensive programming
    perror("ERROR on accept");
    return -1;
  }
  printf("Player connected successfully\n");
  return comm_socket;
}

/* 'BEGIN' the game */
void start_game(int comm_socket) {
  char *message = "BEGIN";
  send_message(comm_socket, message); //send message through comm_socket
  return;
}

/* Recognize join request */
bool recognize_join(int comm_socket, char **player_name) {
  char message[MSG_SIZE];
  receive_message(comm_socket, message);
  if (strncmp(message, "JOIN", 4) == 0) {
    for (int i = 0; i < strlen(message); i++) {
      if (message[i] == ' ') {
        *player_name = &message[i + 1];
        return true;
      }
    }
  }
  return false;
}

/* Reveal a card to the player */
void net_deal_card(int comm_socket, int is_dealer_card, card_t *card) {
  char message[MSG_SIZE];
  if (is_dealer_card == 1) {
    strcpy(message, "DEALER ");
  } else if (is_dealer_card == 0) {
    strcpy(message, "CARD ");
  }
  strcat(message, get_card_string(card));
  send_message(comm_socket, message);
  return;
}

/* Request player 'DECISION' */
void request_decision(int comm_socket) {
  char *message = "DECISION";
  send_message(comm_socket, message);
  return;
}

/* Receive player's 'DECISION' */
int receive_decision(int comm_socket) {
  char message[MSG_SIZE];
  receive_message(comm_socket, message);
  if (strcmp(message, "HIT") == 0) return 1;
  if (strcmp(message, "STAND") == 0) return 0;
  fprintf(stderr, "ERROR: Invalid decision: %s\n", message);
  return -1;
}

/* Send Results to player */
void send_result(int comm_socket, int result) {
  char message[MSG_SIZE];
  if (result == 1) {
    strcpy(message, "RESULT WIN\n");
  } else if (result == 0) {
    strcpy(message, "RESULT PUSH\n");
  } else if (result == -1) {
    strcpy(message, "RESULT LOOSE\n");
  } else if (result == -2) {
    strcpy(message, "RESULT BUST\n");
  } else {
    fprintf(stderr, "[-] Invalid result: %d\n", result);
  }
  send_message(comm_socket, message);
  return;
}

/* Send 'QUIT' message */
void end_game(int comm_socket) {
  char *message = "QUIT";
  send_message(comm_socket, message);
  return;
}

/* ========================================================================== */
/* ============================ Player Methods ============================== */
/* ========================================================================== */

/* Connect to an open game session */
int connect_to_session(char *hostname, int port) {
  int comm_sock = tcp_ipv4_socket();

  struct hostent *hostp = gethostbyname(hostname);
  if (hostp == NULL) {
    fprintf(stderr, "ERROR, Host: %s, not found.\n", hostname);
    return -1;
  }
  printf("Found host '%s'\n", hostname);

  // Initialize the  address
  struct sockaddr_in addr;
  memset(&addr, '\0', sizeof(addr));  // clear the address
  addr.sin_family = AF_INET;          // IPv4
  bcopy(hostp->h_addr_list[0], &addr.sin_addr, hostp->h_length);
  addr.sin_port = htons(port);

  // Connect to the server
  connect(comm_sock, (struct sockaddr *)&addr, sizeof(addr));
  printf("Connected to server '%s' on port %d\n", hostname, port);

  return comm_sock;
}

/* Send a 'JOIN' message to the dealer */
void request_to_join(int comm_socket, char *player_name) {
  char *message = malloc(BUFFER_SIZE);
  sprintf(message, "JOIN %s", player_name);
  send_message(comm_socket, message);
  sleep_26();
  free(message);
}

/* Send a 'HIT' or 'STAND' message to the dealer */
void send_decision(int comm_socket, int decision) {
  // 0 = stand, 1 = hit
  char *message;
  if (decision == 0) {
    message = "STAND";
  } else if (decision == 1) {
    message = "HIT";
  } else {
    printf("ERROR: invalid decision. Allowed: 0 -> STAND, 1 -> HIT\n");
    return;
  }
  send_message(comm_socket, message);
  return;
}

/* Receive a message from the dealer about current game state */
int player_state(int comm_socket, char **result, char **card_string) {
  char message_array[BUFFER_SIZE];
  char *message = &message_array[0];
  receive_message(comm_socket, message);

  if (strcmp(message, "BEGIN") == 0) return 0;

  if (strncmp(message, "CARD", 4) == 0) {
    // skip past "CARD "
    for (int i = 0; i < 5; i++) {
      message++;
    }
    strcpy(*card_string, message);
    return 1;
  }

  if (strncmp(message, "DEALER", 6) == 0) {
    // skip past "DEALER "
    for (int i = 0; i < 7; i++) {
      message++;
    }
    strcpy(*card_string, message);
    return 2;
  }

  if (strcmp(message, "DECISION") == 0) return 3;

  if (strncmp(message, "RESULT", 6) == 0) {
    // skip past "RESULT "
    for (int i = 0; i < 7; i++) {
      message++;
    }
    strcpy(*result, message);
    return 4;
  }

  if (strcmp(message, "QUIT") == 0) return 5;

  fprintf(stderr, "[-] Invalid message from server: %s\n", message);
  return -1;
}


/********** local functions **********/
static void sleep_26() {
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 3 * 1000000;
    nanosleep(&ts, &ts);
}
