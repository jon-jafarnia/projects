# Jon Jafarnia's Implementation Spec for CS50 TSE Querier
## Implementation Specification

In this document we reference the [Requirements Specification](on course webpage) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
Implementation may include many topics; not all are relevant to the TSE or the Querier.
Here we focus on the core subset:

-  Control flow: pseudo code for overall flow
-  Detailed function prototypes and their parameters

## Control flow

### void validate_paramaters(int argc, char *argv[]);
      validates all of the paramters and exits non-zero if error.

### void parse_query(index_t* index, char* page_directory);
      reads input from stdin
      tokenizes input and passes that through process query if valid
      calls rank to rank and print score of counters object returned from process query
      free up memory and prompt user for another input

### char** tokenize(char* given_words, int* word_count);
      checks to make sure all valid characters
      normalize input
      chop the word using method explained on webpage
      print the array
      set pointer to word_count equal to number of words
      check first and last word are valid
      return words array

### void print_array(char** words, int nWords);
      simply prints the user inputted query in correct form
### bool check_first_and_last(char** words, int nWords);
      checks first character isn't and or or
      checks last character isn't and or or

### counters_t* process_query(index_t* index, char** array, int num_words);
      // Create two counters initially set to NULL:
    counters_t *andSequence // holds result of current AND operations
    counters_t *orSequence  // accumulates the final result combining OR sequences
    shortCircuit = false    // stop early if AND sequence will fail

   Loop over all words in query
      if word == OR 
         // orSequence holds final result, merge in andSequence seen so far
         // think of stepping back after previously stepping away to calculate AND
         MERGE(andSequence, orSequence) 
         shortCicuit = false  //reset shortCircuit
         continue
      if shortCircuit 
         // at least one word returned no results, so AND sequence will fail
         // skip the words until see OR and shortCircuit is reset
         continue 
      if word == AND
         continue to next word // same as if AND wasn't there
      else found non operator word 
         // think of stepping away to calculate AND sequence
         get pages for this word with match = index_find(word)
         if match == NULL  // no pages contain this query word
            shortCircuit = true // AND will fail for next word, so skip it and others until see OR
            if andSequence != NULL 
               // because this word returned no pages, drop everything AND'ed so far
               delete andSequence
               andSequence = NULL
         else  // found counters for this word in index
            if andSequence == NULL // this is the first word of AND sequence
               andSequence = new counters
               UNION(andSequence, match)  //essentially copies match to andSequence
            else // not first word in AND sequence
               INTERSECT(andSequence, match) //intersect this word's pages 

   MERGE(andSequence, orSequence)
   return orSequence

### void merge(counters_t **and_sequence, counters_t **or_sequence);
      // push temporary result andSequence to final result orSequence
      // also reset andSequence
      if andSequence != NULL
            if orSequence == NULL
                  orSequence = new counters
            UNION(orSequence, andSequence) //union andSequence into orSequence
            delete andSequence and set to NULL

### void union_counters(counters_t *or_sequence, counters_t *other_sequence);
      // union orSequence and otherSequence by adding counts for matching keys
      // store results in orSequence
      counters_iterate(otherSequence, orSequence, unionFunction)

### void unionFunction(void* arg, const int key, const int count);
      // update arg counters 
      wordCount = counters_get(arg, key) // returns 0 if key not found
      counters_set(arg, key, count + wordCount)

### void intersect(counters_t *or_sequence, counters_t *other_sequence);
      // intersect orSequence and otherSequence by taking of counts for matching keys
      // store results in orSequence
      struct twocounters args = {orSequence, otherSequence}
      counters_iterate(orSequence, args, intersectFunction)

### void intersectFunction(void* arg, const int key, const int count);
      counters_set(arg->orSequence, key, min(count, counters_get(args->otherSequence, key)))

### int fileno(FILE *stream);
      function given by webpage to use prompt

### static void prompt(void);
      function given by webpage to only ask for query if user is providing stdin
### int min(int temp1, int temp2);
      find smaller or 2 numbers and return it

### void rank(counters_t* counters_rank, char* page_directory);
      iterate over counters and get total_items
      check total_items
      print how many docs match total_items
      make a struct keyscore and pass it through counters_iterate with a function to find biggest page
      print the page
      set biggest counter to 0

### static void count_items(void* arg, const int key, int count);
      function to count total number of items with score > 0

### static void find_biggest(void* arg, const int key, int count);
      if the biggest score is less than current counters item score, then switch them

### void print_counters(char* page_directory, struct keyscore* score_to_print);
      loads a webpage
      checks load
      print the counters object ad webpage in correct form
      delete webpage

## Testing plan

See DESIGN.md