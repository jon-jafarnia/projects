# CS50 Final Project
## Design Specification

According to the [BlackJack Requirements Spec] on the course webpage, the *Blacjack* is a collection a programs that work in unison using reinforcement learning in order to create a system that will make the best possible decision in a blackjack game.

This specification is written with the purpose of describing the abstract data structures and design decisions for the final project.
It will not repeat any information already discussed in README.md or REQUIREMENTS.md

The blackjack has very little user interface, and it prints all of its decisions and the results of each game for the user to see. 

### Inputs and outputs

**Input**: we have no inputs besides those of the command line, which were discussed for both player and dealer in the REQUIREMENTS.md. 

**Output**: We print the cards dealt and the decisions made by player. If we are in training then we create a file using q that will be saved and can be later loaded to allow the program to play with trained data. 

### Major data structures

* A key data structure used by both *Player* and *Dealer* is *card_t*, which contains a string form of the card (Rank of Suit) and the integer rank of the card.
* Another key data structure is *deck_t*, which uses an array of booleans to indicate which cards in a card array have already been dealt. 
* Player uses *hand_t* to keep track of information relevant to the player, including the player's card and the dealer's face up card. The hand module also allows functionality such as retreiving the total value of a player's cards. 
* Player also uses *q_t* to maintain its reinforcement learning data. A *q_t* holds a current average for each state and the number of times each state has occured. 


### Player Design

- The player will first parse its command line arguments, exiting and printing error and usage when necessary. 
- Inside the loop that handles messages from the dealer, the player will keep a hand that updates as the player receives card. At the end of that round, the hand is reset. The player keeps a single q_t that is updated after each hand, ensuring that it updates with each state in the previous hand. At the end of play, that q_t is saved to the given file. 
- In training mode, the player will randomly choose to HIT or STAND in each state. In playing mode, the player will choose the option that has a greater average reward in `q`. 
- `q` wil keep track of 4 aspects of each state:
    -   the player's cards' value
    -   the dealer's face-up card's value
    -   whether the player has the ability to convert a 'hard' ace to a 'soft' ace (is an ace currently being used as an 11?)
    -   whether the player chose to hit or stand
- The third metric is in addition to the assigment discription and should create a competitive edge. 

### Dealer Design and Functionality

* The dealer runs from the main function, but via design choices, we chose to split up its actions into countless functions to make the code easier to read and debug.
* Utilizing pointer variables, we were able to track changes made to variables in  various helper functions and maintain their values
*Dealer only accomodates 1 player

The `dealer` will run as follows:
    
    parse command_line arguments appropriately

    int player_socket, dealer_socket;
    start the session creating a port with the command-line argument port (ex. 8095)
    player_socket = 
    while the player socket is receiving messages
        wait for the player to join the game

        play games until number of games passed in has been played
            send a message to player that game is beginning
            shuffle the deck 
            deal 2 cards to player
            deal 1 card to dealer
            ask for a decision from player
            IF hit:
                continue to deal cards until he either busts or stands
            IF stand:
                dealer deals cards to himself until obligated to stop by blackjack rules or he busts.
            send `result` message to player once game is over
        send `quit` message to player
    close sockets and shut down server
### Network Design
* A module that handles the communication between player and dealer through TCP/IP sockets.
* It ensures that messages sent adhere to the protocol specified in the [project webpage](https://www.cs.dartmouth.edu/~tjp/cs50/project/index.html)
* Functions `start_session` and `accept_player` start off the communication and connect the 2 sockets
* Once the session has been created and players accepted. communication occurs utilizing functions `send_message` and `receive_message` to send messages about cards being sent `receive_decision` and `send_decision` are in regards to decisions made by player to hit or stand.
* Lastly, closing occurs as `end_game` send the final QUIT message to player.

### Card Design

* A module with a struct representing a single card in a blackjack game with a `string_value` and an int `value`.
* Card also offers various helper functions such as `get_card_string(card_t* card)` that help `Dealer` and `Player` get the information they need to run.
* The module has multiple different ways to create a new card in order to accomodate unique situations we encounter in different classes.

### Deck Design

* A module with a struct that represents the entire deck of cards, primarily used by dealer. 
* Includes an array of card_t* objects and array of booleans indicating whether the card has been dealt or not.
* Functions like `deck_shuffle` and `deck_deal` represent the actions that would occur to a phsyical deck during a blackjack game.

### Hand Design

* A module with a struct catered towards player.
* A card_t* pointer representing initial dealer card needed for a `Player` to make decisions and array of card_t* objects for player.
* Functions such as `hand_add_player_card` represent the actions that would occur to a phsyical hand during a blackjack game.
* Functions such as `hand_is_player_using_hard_ace` calculate critical information for our implementation of q. 

### Dealer_Hand Design

* A module with a struct containing exclusively an array of card_t* objects, primarily used for Dealer.
* An implementation suited for `Dealer` who only tracks his hand.


### User interface

* Many statements are printed to the User Interface, as the user can observe the actions/results of each game that is played.
    `Dealer` UI single game example:
    ```
    Sending message: BEGIN
    Deck shuffled
    Creating new hand
        hand created
    Sending message: CARD Two of Clubs
    Sending message: CARD Seven of Spades
    Sending message: DEALER Six of Diamonds
    Cards dealt
    Sending message: DECISION
    Player stand
    (Six of Diamonds, Nine of Diamonds, Nine of Hearts)
    Current player hand:
    (Two of Clubs, Seven of Spades)
    Player hand value: 9
    Current dealer hand:
    (Six of Diamonds, Nine of Diamonds, Nine of Hearts)
    Dealer hand value: 24
    Player wins
    Sending message: RESULT WIN
    ```

    `Player` UI single game example:
    ```
    Beginning new game.
        Received player card: Two of Clubs
        Received player card: Seven of Spades
        Received dealer card: Six of Diamonds
    Sending message: HIT
            Received player card: Nine of Diamonds
    Sending message: STAND
    Received result: WIN
    ```

### Functional decomposition into modules

We anticipate the following modules or functions in our player class:

 1. *main*, which parses arguments, initailizes other modules, and handles the communication between the dealer and player
 2. *indexBuild*, which builds an in-memory index from webpage files it finds in the pageDirectory;


We anticipate the following modules or functions in our dealer class:

1. *main*, which parses arguments, initializese other modules and runs the game simulation;
2. *session_run*, simulates a single game run, initializing other modules, and handling communication from dealer and player;
3. *deal_card_to_hand*, deals the card to hand from deck and adds the card to the hand;
4. *check_hands*, checks the player and dealer hands and prints their current values;
5. *check_send_result*, checks the player score and then  sends appropraite result message to player;
6. *dealer_response*, handles the dealer response, in terms of playing cards, to the player decision that it receives
7. *dealer_take_card_decision*, helper function for dealer_response that decides whether or not dealer will take the card;


And some helper modules that provide data structures (these can all be found in common):

 1. *card*, a module providing the data structure to represent the in-memory card, this also contains functions to get the cards string, value, create new cards, and delete cards;
 2. *dealer_hand*, a module providing the data structure to represent the dealers hand. This module adds cards, gets values, and prints hands in order to work optimally;
 3. *deck*, a module providing the data structure to represent the in-memory deck, this also contains functions to build the deck, shuffle, deal cards, and delete the deck;
 4. *hand*, a module providing the data structure to represent the players hand. This module adds cards, gets values, and prints hands in order to work optimally;
 5. *network*, a module that allows the dealer and player to both send and receive mesages;
 6. *q*, data structure that uses reinforcement learning to save the decisions and results to a file. This also has a laod function that allows the user to load a created q file;


    
