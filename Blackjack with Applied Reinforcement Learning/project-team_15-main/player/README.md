# Player module
## CS50 Final Project

### Overview

This module implements the player for the game of Blackjack. 
The player, running from the command line, is responsible for connecting to the dealer, receiving cards from the dealer, and playing the game of Blackjack. 
The player also keeps track of its own hand and the dealer's hand.
It tracks how its decision to hit or stand affects the outcome of the game, and uses this information to make better decisions in the future. 

See the specification documents for more details on how the player as well as other modules work.

- [REQUIREMENTS.md](REQUIREMENTS.md)
- [DESIGN.md](DESIGN.md)
- [IMPLEMENTATION.md](IMPLEMENTATION.md)

### File Structure

- [player.c](player.c): The main file for the player module. This file contains the main function and the functions that implement the player's functionality.
- [testing.sh](testing.sh): A script that runs the player module in a testing environment.
- [testing.out](testing.out): The output of the testing script.
- [Makefile](Makefile): A makefile that compiles the player module.
- [q.txt](q.txt): A file that contains a q_matrix that stores the player's decision-making information. This file is used to test the player's ability to learn from previous games.