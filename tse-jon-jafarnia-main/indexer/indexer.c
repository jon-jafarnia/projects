/* 
 * indexer.c - a C program that creates an indexPage after taking in the crawlers output
 *
 * We will use the indexer for the final stage of our tiny search engine. 
 * It plays a vital role in organizing the files collected.
 * 
 * input: A valid page directory(should be crawler output) and a indexFilename to write to
 * output: A document called indexFilename that holds one word per line following by a however many docID, count occurences
 *
 * compile: make
 * 
 * Jon Jafarnia, Fall 2022
 * CS 50, Fall 2022
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "../libcs50/counters.h"
#include "../libcs50/hashtable.h"

//Main function, calls validate_paramaters, then indexBuild, save index to file, then clean up data structures then returns 0
// should be called ./indexer pagedirectory, indexFilename
int main(const int argc, char* argv[]);

// Make sure all paramters are valid
bool validate_paramters(char* argv[]);

//Builds the index taking pagedirectory and indexFilename as parameters
index_t* index_build(char* argv[]);

//Takes a valid index, page, and docID
void index_page(index_t* index, webpage_t* page, int docID);


int main(const int argc, char* argv[]){
      //check number of arguments, print error and exit if invalid
      if (argc < 3 || argc > 3){
            fprintf(stderr, "Arguments out of range. Give 2 \n");
            exit(1);
      }
      //call validate_paameters to check if the parameters are valid, print error if false
      if (validate_paramters(argv) == false){
            fprintf(stderr, "Failed to validate parameters \n");
            exit(2);
      }

      //make the index calling index_build
      index_t *index = index_build(argv);

      //save the index calling index_save
      index_save(index, argv[2]);

      //delete the index
      index_delete(index);
      //return zero if process correctly works
      return 0;
}

//boolean to check if parameters are valid
bool validate_paramters(char* argv[]){
      //call pagedir_validate which checks if the directory is one built by crawler
      if(pagedir_validate(argv[1]) == false){
            printf("Invalid Directory\n");
            return false;
      }
      //check if indexFilename can be written to
      FILE *fp = fopen(argv[2], "w");
      if (fp == NULL){
            printf("Can not write to file\n");
            return false;
      }
      fclose(fp);
      //return true if all parameters valid
      return true;
}

index_t* index_build(char* argv[]){
      // create new index with index_new(number_slots);
      index_t *temp_index = index_new(1000);
      // set docID =1;
      int docID = 1;
      //set string for page directory
      char *page_directory = argv[1];
      //Create a webpage page
      webpage_t *page;
      // while page = pagedir_load(pageDirectory, docID) is not NULL
      while ((page = pagedir_load(page_directory, docID)) != NULL){
      //       index_page(index, page, docID) //make entry for each word in this page
            index_page(temp_index, page, docID);
      //       webpage_delete(page)
            webpage_delete(page);
      //       docID++
            docID++;
      }
      // return index
      return temp_index;
}


void index_page(index_t* index, webpage_t* page, int docID){
      //set position to 0
      int pos = 0;
      // create pointer to word
      char* word;
      // while word = webpage_getNextWord(page, &pos) is not NULL
      while((word = webpage_getNextWord(page, &pos)) != NULL){
      //       normalizeWord(word) if length > 2
            if (strlen(word) > 2){
                  //call normalize word on word pointer
                  normalizeWord(word);
            //       get counters for this word from index with index_find(index, word)
                  counters_t* temp_counter = index_find(index, word);
            //       if word not in index
                  if(temp_counter == NULL){
            //             create new counters with counters_new()
                        temp_counter = counters_new();
            //             insert empty counters into index with index_insert(index, word, counters)
                        index_insert(index, word, temp_counter);
                  }
            //       increment word count with counters_add(counters, docID)
                  counters_add(temp_counter, docID);
            }
            //free word
            free(word);
      }
}