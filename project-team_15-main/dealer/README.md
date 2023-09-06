# Dealer module
## CS50 Final Project

### Overview

This module implements the dealer for the game of Blackjack. 
The dealer, running from the command line, is responsible for initializing the game,
creating the deck, and dealing cards to the players. The dealer also keeps track of the
players' hands and determines the winner of the game according to the rules of Blackjack.

See the specification documents for more details on how the dealer as well as other modules work.

- [REQUIREMENTS.md](REQUIREMENTS.md)
- [DESIGN.md](DESIGN.md)
- [IMPLEMENTATION.md](IMPLEMENTATION.md)

### File Structure

- [dealer.c](dealer.c): The main file for the dealer module. This file contains the main function and the functions that implement the dealer's functionality.
- [testing.sh](testing.sh): A script that runs the dealer module in a testing environment.
- [testing.out](testing.out): The output of the testing script.
- [Makefile](Makefile): A makefile that compiles the dealer module.
- dealer-given is a binary file that is given to us by the course staff. It is used to test our player module (prior to our own dealer implementation) and benchmark our dealer. It is used to check that we are playing the game with the same rules and network protocol as the course staff.