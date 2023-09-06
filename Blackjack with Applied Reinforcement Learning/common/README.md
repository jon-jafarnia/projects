# Common library for the Blackjack project
## CS50 Final Project - Team 15

### Overview

This module implements the common library for the game of Blackjack. The common library is used by both the player and the dealer. It contains functions that are used by both modules, such as functions that implement the network protocol, functions that implement the game logic, and functions that implement the data structures used by the player and the dealer. The common library is compiled into a static library `common.a` that is used in compiling the player and the dealer.

See the specification documents for more details on how the common library as well as other modules work.

- [REQUIREMENTS.md](REQUIREMENTS.md)
- [DESIGN.md](DESIGN.md)
- [IMPLEMENTATION.md](IMPLEMENTATION.md)

### File Structure

- [card.c](card.c): This file contains the functions that implement the card data structure.
- [card.h](card.h): This file contains the header file for the card data structure.
- [dealer_hand.c](dealer_hand.c): This file contains the functions that implement the dealer_hand.
- [deck.c](deck.c): This file contains the functions that implement the deck data structure.
- [deck.h](deck.h): This file contains the header file for the deck data structure.
- [hand.c](hand.c): This file contains the functions that implement the hand data structure.
- [hand.h](hand.h): This file contains the header file for the hand data structure.
- [network.c](network.c): This file contains the functions that implement the network protocol.
- [network.h](network.h): This file contains the header file for the network protocol.
- [q.c](q.c): This file contains the functions that implement the q_matrix data structure.
- [q.h](q.h): This file contains the header file for the q_matrix data structure.
- [Makefile](Makefile): A makefile that compiles the common library.