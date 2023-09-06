# Requirements

## Overview

This document describes the requirements for the project.

## Dealer
The dealer shall:
- run from the command line with usage syntax 
`./dealer number_games port_number`
- validate that it received the proper number of command-line arguments and that
  * number_games is an integer greater than 0
  * port_number is a valid integer. for our team, numbered 15, the port_number we used was 8095 (8080+15).
- set up a server socket listening for clients (players) to connect
- once a client connects, pass messages back and forth over socket as shown in diagram below, and
- run the game (to be described in design and implementation specs)
- calculate the game’s result (WIN, LOOSE, BUST, or PUSH) and send a message to the client
- reset and play again (you decide on how many games to play)
- send a QUIT message to the client when done, exitt 0 out of the program.
- exit with an error message to stderr and non-zero exit status when
  * out of memory
  * invalid command line arguments, as discussed above.


## Player
The player shall: 
- run from the command line with usage syntax
`./player15 [player_name] [host] [port] [training_mode_on] [save_q_file] [load_q_on] OPTIONAL: [load_q_file]`
  - player_name: the player's name
  - host: the dealer's IP (can be localhost)
  - port: the port number for use by both dealer and player
  - training_mode_on: a boolean denoting whether the program shall train or play from trained data
  - save_q_file: the file that q will be saved to
  - load_q_on: a boolean denoting whether q will be loaded from a file
  - load_q_file: if load_q_on is true, a user also includes the file that q will be loaded from, load_q_file
- validate that it receives the correct number of arguments
- ensure that the port number is a valid port number
- check that boolean command line arguments equal exactly "true" or "false"
- if command line specifies to load a `q` from file, then do so; else, create a new one
- continously loop through messages sent from the server, properly updating `q` at the end of each hand
- print each action that happens in the game (player receives card, player wins, etc.)

## Network
The network module shall facilitate communication between the dealer and player maintaining to the following protocol:
- Once a TCP connection is establishedthe player shall send a request to join the game: `JOIN [player_name]`
- Once the dealer receives the request, it shall send a message to the player: `BEGIN`
- The dealer shall then send the player two messages showing the player's cards: `CARD [Rank] of [Suit]` and `CARD [Rank] of [Suit]`
- The dealer shall then send the player a message showing the dealer's card: `CARD [Rank] of [Suit]`
- The dealer shall then send the player a message asking for a decision: `DECISION`
- The player shall then send the dealer a message with the player's decision: `HIT` or `STAND`
- If the player stands, the dealer sends the result of the game: `RESULT WIN`, `RESULT LOOSE`, `RESULT BUST`, or `RESULT PUSH`
- If the player hits, the dealer shall send the player a message showing the player's new card: `CARD [Rank] of [Suit]`
- If the player busts, the dealer shall send the result of the game: `RESULT BUST`
- Otherwise, the dealer shall send the player a message asking for a decision: `DECISION` repeating the process above until the player stands or busts
- To end the game, the dealer shall send the player a message: `QUIT`


