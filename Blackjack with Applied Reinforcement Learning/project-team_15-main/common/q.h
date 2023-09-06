/* 
q.h
Team 15
CS50 Fall 2022
*/

#ifndef __Q_H
#define __Q_H

#include "hand.h"


/**************** struct ****************/
typedef struct q q_t;


/**************** public functions ****************/
/*
 * returns a pointer to a new, empty q
 */
q_t* q_new();

/*
 * saves q to the given file_path
 */
void q_save(q_t* q, char* file_path);

/*
 * returns a pointer to a new q filled with values from file_path
 */
q_t* q_load(char* file_path); 

/*
 * puts values in q for the given hand and reward
 * all cards in hand except the first 2 are deleted in the process; not important because hand is never used again
 */
void q_record_hand(q_t* q, hand_t* hand, char* result);

/*
 * returns 1 if player should hit at current state, 0 if player should stand
 */
int q_get_decision(q_t* q, hand_t* hand);

/*
 * frees a q
 */
void q_delete(q_t* q);

#endif