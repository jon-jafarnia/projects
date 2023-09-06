//  * Jon Jafarnia, CS 50, Fall 2022
#ifndef __INDEX_H
#define __INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"

/**************** global types ****************/
  // opaque to users of the module
typedef hashtable_t index_t;

/**************** wrapper functions from hashtable ****************/

//See hashtable_new
index_t *index_new(const int number_slots);

//See hashtable_insert
bool index_insert(index_t* index, const char* key, void* item);

//See hashtable_find
void* index_find(index_t* index, const char* key);

//Calls hashtable delete, but uses helper function delete_helper found later in this file
void index_delete(index_t* index);



/**************** functions ****************/

/**************** index_save ****************/
/* Boolean that returns true if we succesfully write to a file
 *
 * Caller provides:
 *   valid pointer to a index, and file (index_file) they would like to write to
 *   (must be a unique index_file or you will write over a previous file)
 * We return:
 *   true if we write to a new file, we return false if we can't open the file for writing or if index is null
 * We guarantee:
 *   The user is still responsible for deleting the index after save is called.
 */
bool index_save(index_t* index, char *index_file);

// /**************** index_load ****************/
// /* Save a webpage to a new file inside a given directory.
//  * 
//  * Caller provides:
//  *   valid pointer to a file (index_file) they would like to load from
//  *   (file must be in the correct layout)
//  * We return:
//  *   an index loaded from a file
//  * We guarantee:
//  *   The index will be stored in memory and available for use
//  */
index_t* index_load(char *index_file);

/**************** delete_helper ****************/
/* void serves as helper function for delete
 *
 * Caller provides:
 *   valid pointer to a counters item they would like deleted
 * set counters_item to item
 * counters_t* counters_item = item;
 * If item is valid counter then free it
 * We guarantee:
 *   The counters item is deleted
 */
void delete_helper(void* item);

/**************** function_to_print_key ****************/
/* void serves as helper function for printing the key
 *
 * Caller provides:
 *   Nothing this is called in save as part of an iterate function
 * functionToPrintKey
 * print key
 * iterate over each counter with counters_iterate(counter, fp, functionToPrintCounter)
 * We guarantee:
 *   The key(a word) is printed
 */
void function_to_print_key(void *arg, const char *key, void *item);

/**************** function_to_print_counters ****************/
/* void serves as helper function for printing the key
 *
 * Caller provides:
 *   Nothing this is called in function_to_print_key and is used in an iterate function
 * functionToPrintCounter
 * print count
 * We guarantee:
 *   The count is printed in correct format
 */
void function_to_print_counters(void *arg, const int key, int count);

#endif // __INDEX_H
