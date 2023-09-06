# Jon Jafarnia's Implementation Spec for CS50 TSE Indexer
## Implementation Specification

In this document we reference the [Requirements Specification](REQUIREMENTS.md) and [Design Specification](DESIGN.md) and focus on the implementation-specific decisions.
Implementation may include many topics; not all are relevant to the TSE or the Indexer.
Here we focus on the core subset:

-  Data structures
-  Control flow: pseudo code for overall flow
-  Detailed function prototypes and their parameters
-  Error handling and recovery
-  Testing plan

## Data structures 

We created an index data structure: An index essentially serves as a wrapper for hashtable and holds counter items. The key is an index and item is a counters data type holding (key(word), count(number of times word appeared))

The size of the index (slots) is impossible to determine in advance, so we use 1000.  (NOTE: our hashtable implementation does not grow in size as hashtables did in CS 10 to keep a low load factor, but a starting size of 1000 works well for this assignment)

## Control flow

The Indexer is implemented in one file `indexer.c`, with three functions.

### 1. main

The `main` function simply calls `validate_parameters` and `index_build`, then exits zero.

### 2. validate_parameters

Given arguments from the command line, extract them into the function parameters; return only if successful.

* Makes sure that the first paramter is valid `pageDirectory` with crawler output. Uses pagedir function `pagedir_validate`
* for `indexFilename`, simply make sure it is a valid file to write to.
* if any trouble is found, print an error to stderr and exit non-zero.

### 3. index_build

Loop over all webpages stored by crawler in pageDirectory, index those pages by looking for each word on each page, make entry in index (hashtable) for each word.
Pseudocode:

      create new index with index_new(number_slots);
      set docID =1;
      load webpage with webpage_t page = pagedir_load(pageDirectory, docID)
      while page not NULL
            index_page(index, page, docID) //make entry for each word in this page
            webpage_delete(page)
            docID++
            page = pagedir_load(pageDirectory, docID)
      return index

### 4. index_page

Find words on a single page, create a counter for each word, make entry into index (hashtable)
Pseudocode:
      get next word on page with webpage_getNextWord()
      while word not NULL
            normalizeWord(word) if length > 2
            get counters for this word from index with index_find(index, word)
            if word not in index
                  create new counters with counters_new()
                  insert empty counters into index with index_insert(index, word, counters)
            increment word count with counters_add(counters, docID)
            word = webpage_getNextWord()
      free(word)

## Other modules

### pagedir
      See pagedir.h for details on each function

### word
      See word.h for detail on the function

### index
      See index.h for details on each function

## Error handling and recovery

All the command-line parameters are rigorously checked before any data structures are allocated or work begins; problems result in a message printed to stderr and a non-zero exit status.

## Testing plan

Here is an implementation-specific testing plan.

### Unit testing

There are 4 units (indexer, pagedir, index, word).
The indexer calls all of the other units.
The pagedir, and word units are tiny; they could be tested using a small C 'driver' to invoke its functions with various arguments, but it is likely sufficient to observe their behavior during the system test.

### Regression testing

The testing.sh demonstrates most cases of the indexer and index files working, but the indexertest.c file containins unit testing that shows the index_load function working.

### Integration/system testing

We write a script `testing.sh` that invokes the indexer several times, with a variety of command-line arguments.
First, a sequence of invocations with erroneous arguments, testing each of the possible mistakes that can be made.
Second, a run with valgrind over a moderate-sized test case.
Third, runs over a few of the cs50 websites.
Run that script with `bash -v testing.sh` so the output of indexer is intermixed with the commands used to invoke the indexer.
Verify correct behavior by studying the output, and by sampling the files created in the respective pageDirectories.