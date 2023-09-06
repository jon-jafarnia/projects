/* 
 * index.c - CS50 'index' module
 *
 * see index.h for more information.
 *
 * Jon Jafarnia, October 2022
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"
#include "../libcs50/file.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"


/**************** global types ****************/
typedef hashtable_t index_t;

/**************** wrapper functions from hashtable ****************/

/**************** index_new() ****************/
/* see index.h for description */
index_t *index_new(const int number_slots){
  return hashtable_new(number_slots);
}

/**************** index_insert() ****************/
/* see index.h for description */
bool index_insert(index_t* index, const char* key, void* item){
  return  hashtable_insert(index, key, item);
}

/**************** index_find() ****************/
/* see index.h for description */
void* index_find(index_t* index, const char* key){
  return hashtable_find(index, key);
}

/**************** index_delete() ****************/
/* see index.h for description */
void index_delete(index_t* index){
  hashtable_delete(index, delete_helper);
}

/**************** index_save() ****************/
/* see index.h for description */
bool index_save(index_t* index, char *index_file){
//   safety checks on index and indexFilename
  if (index != NULL){
//   open fp on indexFilename for writing (with safety check that it opened)
    FILE *fp = fopen(index_file, "w");
    if (fp == NULL){
//   iterate over hashtable with hashtable_iterate(index, fp, functionToPrintKey) 
      fprintf(stderr, "File %s failed to open\n", index_file);
      return false;
    }
    // iterate through the table using function_to_print_key
    hashtable_iterate(index, fp, function_to_print_key);
    fclose(fp);
    return true;
  }
  //returning false here because this means that index was passed as a NULL (invalid) paramater
  return false;
}

/**************** index_load() ****************/
/* see index.h for description */
index_t* index_load(char *index_file){
  // safety checks on indexFilename
  // open fp with indexFilename for reading (with safety check that it opened)
  FILE *fp = fopen(index_file, "r");
  if (fp == NULL){
  // check for empty file (can use file_numlines(fp) to get word count because each word is on one line)
    fprintf(stderr, "File %s failed to open\n", index_file);
    return NULL;
  }
  //Make an int lines to figure out how many words in the file
  int lines = file_numLines(fp);
  //int lines = 5;
  //fprintf(stdout, "WAHOO %d\n", lines);
  //exit(4);
  // print error message and return NULL if file contains no words
  if (lines == 0){
    fprintf(stderr, "File %s has no words\n", index_file);
    return NULL;
  }
  // create new index with index_new (how many slots? wordcount/2+1?)
  index_t *temp_index = index_new((lines/2)+1);
  //While loop until reaching end of file
  while (!feof(fp)){
  // read each word in file with file_readWord(fp) 
    char* word = file_readWord(fp);
  // create new counter for word with counters_\new()
    counters_t *temp_counters = counters_new();
  // expect one or more docID/count pairs on line following word
    if (index_find(temp_index, word) == NULL){
      index_insert(temp_index, word, temp_counters);
    }
    int key = 0;
    int count = 0;
  // while line has docID/count pair
    while(fscanf(fp, "%d %d ", &key, &count) == 2){
  //     increment counters with counters_set(counters, docID, count)
      counters_set(index_find(temp_index, word), key, count);
    }
  // //counters now contains one element for each docID
  // add counters to hashtable with hashtable_insert(index, word, counters)
    hashtable_insert(temp_index, word, temp_counters);
  // free(word)
    free(word);
  }

  // close(fp)
  fclose(fp);
  // return index
  return temp_index;
}

/**************** delete_helper() ****************/
/* see index.h for description */
void delete_helper(void* item){
  //set counters_item to item
  //counters_t* counters_item = item;
  //If item is valid counter then free it
  if (item != NULL){
    counters_delete((counters_t *)item);
  }
}

/**************** function_to_print_key() ****************/
/* see index.h for description */
//   functionToPrintKey
//     print key
//     iterate over each counter with counters_iterate(counter, fp, functionToPrintCounter)
void function_to_print_key(void *arg, const char *key, void *item){
  FILE *fp = arg;
  counters_t *temp_counters = item;
  fprintf(fp, "%s", key);
  counters_iterate(temp_counters, fp, function_to_print_counters);
  fprintf(fp, "\n");

}

/**************** function_to_print_counters() ****************/
/* see index.h for description */
//   functionToPrintCounter
//     print count
void function_to_print_counters(void *arg, const int key, int count){
  FILE *fp = arg;
  fprintf(fp, " %d %d", key, count);
}