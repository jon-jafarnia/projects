/* 
 * word - normalize a given word

 * Jon Jafarnia
 * CS 50, Fall 2022
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void normalizeWord(char *word){
      //for each character in word, make it lowercase
      for (int i = 0; i<strlen(word); i++){
            word[i] = tolower(word[i]);
      }

}