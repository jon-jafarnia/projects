/* 
 * word.h - header file for CS50 word module
 *
 * This file simply serves to normalize a word then return it.
 *
 * Jon Jafarnia
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**************** normalizeWord() ****************/
/* void function that serves to make a word lower case. Simply give this function a pointer to a word and it will work.
 *
 * We return:
 *   Nothing. We change the pointer to the word. 
 */
void normalizeWord(char *word);