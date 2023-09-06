//Jon Jafarnia
// executes from a command line with usage syntax
// Call this with ./indextest oldIndexFilename newIndexFilename
// oldIndexFilename is the name of a file produced by the indexer, and
// newIndexFilename is the name of a file into which the index should be written.
// load the index from the oldIndexFilename into an inverted-index data structure.
// create a file newIndexFilename and write the index to that file
//Assume parameters given are all in correct format, thus this file has less defensive programming and testing.
// CS050 Fall 2022

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "../libcs50/counters.h"

int main(const int argc, char* argv[]){
      //check arguments
      if (argc < 3 || argc > 3){
            fprintf(stderr, "Arguments out of range. Give 2 \n");
            exit(1);
      }
      char* oldIndexFilename = argv[1];
      char* newIndexFilename = argv[2];
      index_t *index = index_load(oldIndexFilename);
      index_save(index, newIndexFilename);
      index_delete(index);
      return 0;
}