//Jon Jafarnia

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "../common/index.h"
#include "../common/pagedir.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../common/word.h"
#include "../libcs50/mem.h"

/******** local data types *******/
struct twocts {
  counters_t *result;
  counters_t *another;
};

struct keyscore {
      int key; 
      int score;
} keyscore_t;


void validate_paramaters(int argc, char *argv[]);
void parse_query(index_t* index, char* page_directory);
char** tokenize(char* given_words, int* word_count);
void print_array(char** words, int nWords);
bool check_first_and_last(char** words, int nWords);
counters_t* process_query(index_t* index, char** array, int num_words);
void merge(counters_t **and_sequence, counters_t **or_sequence);
void union_counters(counters_t *or_sequence, counters_t *other_sequence);
void unionFunction(void* arg, const int key, const int count);
void intersect(counters_t *or_sequence, counters_t *other_sequence);
void intersectFunction(void* arg, const int key, const int count);
int fileno(FILE *stream);
static void prompt(void);
int min(int temp1, int temp2);
void rank(counters_t* counters_rank, char* page_directory);
static void count_items(void* arg, const int key, int count);
static void find_biggest(void* arg, const int key, int count);
void print_counters(char* page_directory, struct keyscore* score_to_print);

int main(int argc, char *argv[])
{
      //create index structure
      validate_paramaters(argc, argv);
   

      //load an index
      char* page_directory = argv[1];
      char* index_filename = argv[2];
      index_t *index = mem_assert(index_load(index_filename), "Failed to load an index.\n");
      parse_query(index, page_directory);
      //delete the index
      index_delete(index);
     
      //return zero if no errors.
      return 0;
}


void validate_paramaters(int argc, char *argv[]){
      if(argc != 3){
            fprintf(stderr, "Invalid number of arguments, please give 3\n");
            exit(1);
      }
      //call pagedir_validate which checks if the directory is one built by crawler
      if(pagedir_validate(argv[1]) == false){
            fprintf(stderr, "Invalid Directory\n");
            exit(2);
      }
      //check if indexFilename can be read
      FILE *fp = fopen(argv[2], "r");
      if(fp == NULL){
            fprintf(stderr, "Can not write to file\n");
            exit(3);
      }
      fclose(fp);
}

void parse_query(index_t* index, char* page_directory){
      //prompt if input is stdin
      prompt();
      //make string to hold input
      char* input;
      //for each line in file
      while((input = file_readLine(stdin)) != NULL){
            if(strlen(input) == 0){
                  free(input);
                  input = NULL;
                  fprintf(stdout, "Query:\n");
                  fprintf(stdout, "-----------------------------------------------\n");
                  prompt();
                  continue;
            }
            //make pointer to words_array equal the tokenized input
            int word_count;
            char** words_array = tokenize(input, &word_count);
            //int word_count = get_word_count(input);
            if(words_array != NULL){
                  //process the query
                  counters_t* query_to_rank = process_query(index, words_array, word_count);
                  if(query_to_rank != NULL){
                        //Do the ranking
                        rank(query_to_rank, page_directory);
                        counters_delete(query_to_rank);
                  }
                  else{
                        fprintf(stderr, "No documents match.\n");
                  }
            }
            //free the words_array
            free(words_array);
            fprintf(stdout, "-----------------------------------------------\n");
            prompt();
            free(input);
            input = NULL;
      }
}

char** tokenize(char* inputted_words, int* word_count){

      for(int i =0; i < strlen(inputted_words); i++){
            //if letter is neither a space nor letter then return zero and print error message
            if(!isspace(inputted_words[i]) && !isalpha(inputted_words[i])){
                  fprintf(stderr, "Error: bad character '%c' in query.\n", inputted_words[i]);
                  return NULL;
            }
      }
      //normalize the string
      normalizeWord(inputted_words);

      //make an array with enough slots to hold number of words + 1 
      char** words_array = malloc(sizeof(char*) * ((strlen(inputted_words)+1) / 2));
      //pointers to chop word
      char* curr = inputted_words;
      char* rest = inputted_words;
      //int to hold num words
      int nWords = 0;
      //while curr is not a null character
      while(*curr!= '\0'){
            //shift pointer if valid
            while(*curr != '\0' && !isalpha(*curr)){
                  curr++;
            }
            //if curr is not null
            if(*curr != '\0'){
                  //add it to words array
                  words_array[nWords] = curr;
                  //shift
                  nWords++;
                  //set rest = to curr
                  rest = curr;
                  //move rest to end of word
                  while(isalpha(*rest)){
                        rest++;
                  }
                  //set curr = to rest
                  curr = rest;
                  //insert a null terminator at rest and shift curr
                  if(*rest != '\0'){
                        *rest = '\0';
                        curr++;
                  }
            }
      }
      //print the array
      print_array(words_array, nWords);
      //change pointer to word_count
      *word_count = nWords;
      //check first and last and print error if fail
      if(check_first_and_last(words_array, nWords) == false){
            free(words_array);
            fprintf(stderr, "You can't start or end a query with 'or' or 'and'\n");
            return NULL;
      }
      //return words_array
      return words_array;
}


void print_array(char** words, int nWords){
      //print query
      fprintf(stdout, "Query: ");
      //print each word
      for(int i = 0; i<nWords; i++){
            fprintf(stdout, "%s ", words[i]);
      }
      //line break
      fprintf(stdout, "\n");
}

bool check_first_and_last(char** words, int nWords){
      //if first word is either and or or, then return false
      if(strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0){
            return false;
      }
      //if last word is either and or or, then return false
      if(strcmp(words[nWords-1], "and") == 0 || strcmp(words[nWords-1], "or") == 0){
            return false;
      }
      //return true if it worked
      return true;
}

counters_t* process_query(index_t* index, char** words_array, int num_words){
//     // Create two counters initially set to NULL:
//     counters_t *andSequence // holds result of current AND operations
      counters_t *and_sequence = NULL;
//     counters_t *orSequence  // accumulates the final result combining OR sequences
      counters_t *or_sequence = NULL;
//     shortCircuit = false    // stop early if AND sequence will fail
      bool short_circuit = false;

      // //final check in case word
      // if(words_array == NULL){
      //       fprintf(stderr, "Inavlid input.\n");
      //       free(words_array);
      //       return false;
      // }

//    Loop over all words in query
      for(int i = 0; i < num_words ; i++){
//          if word == OR 
            if (strcmp(words_array[i], "or") == 0){
//                // orSequence holds final result, merge in andSequence seen so far
//                // think of stepping back after previously stepping away to calculate AND
//                MERGE(andSequence, orSequence) 
                  merge(&and_sequence, &or_sequence);
//                shortCicuit = false  //reset shortCircuit
                  short_circuit = false;
//                continue
                  continue;
            }
//          if shortCircuit
            if(short_circuit){
//                // at least one word returned no results, so AND sequence will fail
                  // skip the words until see OR and shortCircuit is reset
//                continue 
                  continue;
            }
//          if word == AND
            if(strcmp(words_array[i], "and") == 0){
//                continue to next word // same as if AND wasn't there
                  continue;
            }
//          else found non operator word 
            else{
      //          // think of stepping away to calculate AND sequence
      //          get pages for this word with match = index_find(word)
                  counters_t* match = index_find(index, words_array[i]);
                  //counters_print(match, stdout);
//                if match == NULL  // no pages contain this query word
                  if(match == NULL){
//                      shortCircuit = true // AND will fail for next word, so skip it and others until see OR
                        short_circuit = true;
//                      if andSequence != NULL 
                        if (and_sequence != NULL){
            //                // because this word returned no pages, drop everything AND'ed so far
            //                delete andSequence
            //                andSequence = NULL
                              counters_delete(and_sequence);
                              and_sequence = NULL;
                        }
                  }
//                else  // found counters for this word in index
                  else{
//                      if andSequence == NULL // this is the first word of AND sequence
                        if(and_sequence == NULL){
//                            andSequence = new counters
                              and_sequence = counters_new();
//                            UNION(andSequence, match)  //essentially copies match to andSequence
                              union_counters(and_sequence, match);

                        }
//                      else // not first word in AND sequence
                        else{
//                            INTERSECT(andSequence, match) //intersect this word's pages
                              intersect(and_sequence, match);
                        }
                  }
            } 
      }
//    MERGE(andSequence, orSequence)
      merge(&and_sequence, &or_sequence);
//    return orSequence
      return or_sequence;
}



// MERGE(andSequence, orSequence):
void merge(counters_t **and_sequence, counters_t **or_sequence){
//     // push temporary result andSequence to final result orSequence
//     // also reset andSequence
//     if andSequence != NULL
      if(*and_sequence != NULL){
//        if orSequence == NULL
            if(*or_sequence == NULL){
//           orSequence = new counters
                  *or_sequence = counters_new();
            }
//        UNION(orSequence, andSequence) //union andSequence into orSequence
            union_counters(*or_sequence, *and_sequence);
//        delete andSequence and set to NULL
            counters_delete(*and_sequence);
            *and_sequence = NULL;
      }
}

// UNION(orSequence, otherSequence)
void union_counters(counters_t *or_sequence, counters_t *other_sequence){
//     // union orSequence and otherSequence by adding counts for matching keys
//     // store results in orSequence
//     counters_iterate(otherSequence, orSequence, unionFunction)
      counters_iterate(other_sequence, or_sequence, unionFunction);
}

// unionFunction(arg, key, count)
void unionFunction(void* arg, const int key, const int count){
      //ERROR WITH THIS FUNCTION< WORD COUNT ALWAYS EQUALS ZERO
//    // update arg counters 
//    wordCount = counters_get(arg, key) // returns 0 if key not found
      int word_count = counters_get(arg, key);
//    counters_set(arg, key, count + wordCount)
      counters_set(arg, key, count + word_count);
}

// INTERSECT(orSequence, otherSequence)
void intersect(counters_t *or_sequence, counters_t *other_sequence){
//   // intersect orSequence and otherSequence by taking of counts for matching keys
//   // store results in orSequence
//   struct twocounters args = {orSequence, otherSequence}
      struct twocts args = {or_sequence, other_sequence};
//   counters_iterate(orSequence, args, intersectFunction)
      counters_iterate(or_sequence, &args, intersectFunction);
}

// intersectFunction(arg, key, count)
void intersectFunction(void* arg, const int key, const int count){
      struct twocts *two = arg;
//     counters_set(arg->orSequence, key, min(count, counters_get(args->otherSequence, key)))
      counters_set(two->result, key, min(count, counters_get(two->another, key)));
}

static void
prompt(void)
{
  // print a prompt iff stdin is a tty (terminal)
  if (isatty(fileno(stdin))) {
    printf("Query? ");
  }
}

int min(int temp1, int temp2){
      //find smaller or 2 numbers and return it
      if (temp1 < temp2){
            return temp1;
      }
      return temp2;
}

void rank(counters_t* counters_rank, char* page_directory){
      //int to hold total counters
      int total_items = 0;
      //iterate over counters and get total items
      counters_iterate(counters_rank, &total_items, count_items);
      //if no items print no docs match
      if(total_items == 0){
            fprintf(stdout, "No documents match.\n");
            return;
      }
      //print how many docs match
      fprintf(stdout, "Matches %d documents (ranked):\n", total_items);
      for(int i = 0; i < total_items ; i++){
            //make struct for current keyscore
            struct keyscore curr = {0, 0};
            //iterate and find biggest
            counters_iterate(counters_rank, &curr, find_biggest);
            //print the current biggest counter
            print_counters(page_directory, &curr);
            //set biggest counter to 0
            counters_set(counters_rank, curr.key, 0);
      }
}

/* count the items > 0 and non-null items in the counters.
 * note here we don't care what kind of item is in counters, but it will be integers.
 */
static void count_items(void* arg, const int key, int count)
{
      //increment pointer to total items if score > 0
      int* nitems = arg;
      if (nitems != NULL && count >0)
            (*nitems)++;
}

static void find_biggest(void* arg, const int key, int count)
{
      //make struct keyscore *curr
      struct keyscore *curr = arg;
      //if biggest score < count then switch curr to new counters item
      if (curr->score < count){
            curr->score = count;
            curr->key = key;
      }
}

void print_counters(char* page_directory, struct keyscore* score_to_print){
      //make a webpage using pagedir_load so that we can access url
      webpage_t* page_for_url = pagedir_load(page_directory, score_to_print->key);
      //make sure pagedir_load worked
      if(page_for_url == NULL){
            fprintf(stderr, "Failed to access webpage.\n");
            exit(6);
      }
      //print counters object in correct form
      fprintf(stdout, "score  %d    doc   %d:    %s\n", score_to_print->score, score_to_print->key, webpage_getURL(page_for_url));
      //delete webpage
      webpage_delete(page_for_url);
}