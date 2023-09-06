
# CS50 Final Project
## Implementation Specification


This specification is written with the purpose of describing the implementation-specific (including assumption) decisions for the final project.
It will not repeat any information already discussed in README.md or REQUIREMENTS.md


***Assumptions***

* An assumption made by dealer was that the "game" played followed the `message parsing` sequence displayed by Pierson lab docs. 
    Otherwise, there were no other assumptions about the game made by the dealer.

* A handful of network functions that only run if connection is established and still open assume that if they are called, that this stipulation upholds.

***Pitfalls***
Here we will address some difficulties that we couldn't figure out. Pierson's 2nd version of dealer-given still didn't work with out valgrind. It only worked when we weren't using valgrind and our sleep function used a very small sleep time (a few milliseconds). If you want to test that, you can change the time value in the sleep_26() function in network.c, then call make common and make player. 


### Data Structures & Functionality
***card***:
* FUNCTIONALITY:
* card_t* card_new(char* card_string);
    take a card string,
    find its respective value,
    put card_string and value into a card struct,
    return card
    
* card_t* card_new_from_suit_and_identifier(char* suit, int identifier);
    find the value that corresponds with the given identifier, 
    find the card_string that corresponds with given identifier, 
    add suit to the card_string, 
    put the value and card_string into a card struct, 
    return the card
    
* card_t* card_new_from_rank(char* rank, char* suit);
    find the value that corresponds with the given rank, 
    find the card_string that corresponds with given rank, 
    add suit to the card_string, 
    put the value and card_string into a card struct, 
    return the card
    
* int get_card_value(card_t* card);

* char* get_card_string(card_t* card);

* bool card_delete(card_t* card);
    safety check on card, 
    free the card_string, 
    free the card, 
    return true
    
* int get_card_string_value(char* card_string);
    compare strings and return respective values if valid
    
* char* get_card_string_from_string(char* card_string);
    compare strings and return correctly formatted string if valid
    
* char* assign_card_name(int value);
    check given value, which is an identifier, and set it equal to a correctly formatted string.

***dealer_hand***:  A module with a struct containing exclusively an array of card_t* objects.
* FUNCTIONALITY:
* hand_t* dealer_hand_new();
* bool hand_add_card(hand_t* hand, card_t* card);
* int hand_get_player_value(hand_t* hand);
* int hand_get_dealer_value(hand_t* hand);
* bool hand_is_player_using_hard_ace(hand_t* hand);
* int hand_get_number_player_cards(hand_t* hand);
* void hand_remove_last_player_card(hand_t* hand);
* void hand_print(hand_t* hand);
* void dealer_hand_delete(hand_t* hand);
* hand_t *hand_new(void);
* bool hand_insert(hand_t *hand, card_t *card);
* int hand_size(hand_t *hand);
* int hand_get_value(hand_t *hand);
* void dealer_hand_print(hand_t *hand);
* hand_t *hand_iterate(hand_t *hand, void *arg, void (*itemfunc)(void *arg, const card_t *card));


***hand***: A module with a struct catered towards player, with a card_t* pointer representing initial dealer card and array of card_t* objects for player
- Hand should be set to NULL for each index that doesn't contain a card. This eases implementation. 
- `hand_get_player_value()` should sum each card's value, valuing ace at 1. If an ace has been seen and the sum is less than or equal to 11, then sum += 10 (we're using a hard ace in this case). 
- `hand_is_player_using_hard_ace()` works similarly, keeping track of whether sum += 10 would occur while tallying a player's hand value. 
- `hand_remove_last_player_card()` is for use in `q`, making the idea of looking back at past states simple. 
- For many functions in hard, it is necessary to loop through the player's cards. It is helpful to whether each is NULL, meaning that it is the first index that doesn't already have a card. It should also check that the hand is not full, comparing the current index to a predetermined maximum player hand size. 

***deck***: 
*FUNCTIONALITY:
* deck_t* deck_new(void);
    malloc and safety check on deck, 
    set in_deck true for all cards, 
    loop through each suit, 
        loop through each number 0-12, 
            add a new card to the deck in correct location, 
    return the deck

* void deck_shuffle(deck_t* deck);
    safety check on deck, 
    do the fisher-Yates shuffle algorithm

* card_t* deck_deal(deck_t* deck);
    safety check on deck, 
    loop until see a card that is in deck, 
        set that card to not in deck, 
        return the card, 
    if no cards in deck return NULL

* void deck_print(deck_t* deck);
    print each card in the deck
    
* bool deck_delete(deck_t* deck);
    safety check on deck, 
    card delete for each card in deck, 
    free deck, 
    return true

***network***: A module that handles the communication between player and dealer through TCP/IP sockets.
FUNCTIONALITY:
* void send_message(int comm_socket, char *message);
* void receive_message(int comm_socket, char *message);
* int start_session(int serve_port);
* int accept_player(int server_socket);
* void start_game(int comm_socket);
* bool recognize_join(int comm_socket, char **player_name);
* void net_deal_card(int comm_socket, int is_dealer_card, card_t* card);
* void request_decision(int comm_socket);
* int receive_decision(int comm_socket);
* void send_result(int comm_socket, int result);
* void end_game(int comm_socket);
* int connect_to_session(char *hostname, int port);
* void request_to_join(int comm_socket, char *player_name);
* void send_decision(int comm_socket, int decision);
* int player_state(int comm_socket, char** result, char** card_string);


### Player Implementation
Upon completing `card`, `hand`, and `q` modules, the player implementation is relatively straightforward. The most difficult part is likely the `q` function `q_record_hand()`, so we give the preudocode here:
    determine what reward each state will put into `q`
    for each state: 
	    if player didn't bust
            then they chose STAND after receiving their last card. insert reward with stand
	    if player busted
            then they did not HIT or STAND after receiving their last card, so do nothing
	    in either of the above cases, the last card in the hand has now been properly handed, so remove it
        for every remaining state in this hand (every situation when there were >=2 cards):
            the player hit 
            insert reward with HIT and remove the last card

HINT: Use a loop to update q at each choice the player made, working backwards. This goes 'back in time' by repeatedly removing the last card the player received. 

`q_get_decision` should retreive 0 for STAND or 1 for HIT – whichever had the greater average reward in `q`. 
 
### Dealer Implementation
    The `dealer` will be implemented as follows:
    
    parse_args to make sure appropriate number & type of arguments

    create sockets for both the player and dealer
    start the session creating a port with the command-line argument
    while messages != NULL
        wait for the player to join the game
    
        played_games = 0
        in_session = false
        while(in_session)
            send_message("BEGIN")
            deck_shuffle()
            deal_card("CARD")
            deal_card("CARD")
            deal_card("DEALER")
            send_message("DECISION")
            receive_message()
            while(hit && !BUST) :
                deal_card("CARD")
                send_message("DECISION")
            receive_message()
            IF stand:
                deal_card("DEALER")
               
            send_message("RESULT")
        send_message("QUIT")
    close(sockets)
    shutdown(server)

### Testing Plan
    `Dealer testing`
        * testing if too many command line arguments
        * testing if command line arguments are incorrect
        * a test with thousands of games played
        * valgrind test in Makefile to ensure no memory leaks or errors




