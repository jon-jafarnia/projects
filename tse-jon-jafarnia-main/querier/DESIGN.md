# CS50 TSE Querier
## Design Specification

According to the [Querier Requirements Spec] on the course webpage, the TSE *querier* is a standalone program that takes in a valid page directory and index file, it then loads that index and asks the user for a query. The querier proceeds to rank the scores of each given webpage and then it prints the ranking of these webpages with their score.

### User interface

The querier has a fair amount of user interface and always begins by asking the user for two arguments, a pageDirectory, and an indexFilename. The user is then prompted to ask for input from stdin.

```
querier pageDirectory indexFilename
```

### Inputs and outputs

**Input**: the querier reads files from an indexFilename and loads the given index into its system. The querier also needs a valid pageDirectory in order to run correctly. The querier proceeds to ask the user for stdin in the form of words with no special characters. The user can use 'and' and 'or' to make their search more specific.

**Output**: We rank and print the webpages according to the requirements. The first ouput will be the pagedirectory that is being accessed to allow user to verify output. This is followed by the query and then the ranking

### Functional decomposition into modules

We anticipate the following modules or functions:

 1. *main*, which parses arguments and initializes other modules;
 2. *indexBuild*, which builds an in-memory index from webpage files it finds in the pageDirectory;
 2. *indexPage*, which scans a webpage document to add its words to the index.

And some helper modules that provide data structures:

 1. *index*, a module providing the data structure to represent the in-memory index, and functions to read and write index files;
 1. *webpage*, a module providing the data structure to represent webpages, and to scan a webpage for words;
 2. *pagedir*, a module providing functions to load webpages from files in the pageDirectory;
 4. *word*, a module providing a function to normalize a word.

### Pseudo code for logic/algorithmic flow

The querier will run as follows:

      check all parameters are valid and then ask for user input
      tokenize the input and if it is all valid, then pass it through the query process. If it is invalid then it will return NULL. If NULL print that no documents matched
      set a counters_t object equal to process query output
      process query goes through each word of the input and adds it to a temporary counters object (and_sequence) using a series of merges, intersects, and unions depending on which word is passed. or_sequence serves as the permanent counters object and is returned at the end of this function
      the process goes back into parse_query and then calls rank in order to rank and print the functions
      rank calls a series of counters_iterate functions in order to find the total number of documents with a score, and print them according to spec
      all memory is freed, and the user is prompted to enter a new query on the next line. This continues until the user calls ^D

### Major data structures

The key data structure is the *index*, mapping from *word* to *(docID, #occurrences)* pairs.
The *index* is a *hashtable* keyed by *word* and storing *counters* as items.
The *counters* is keyed by *docID* and stores a count of the number of occurrences of that word in the document with that ID. 
The *keyscore* is keyed by int *key* which is essentially a docID and stores a score which is a tally of how the document matched the query.
The *twocts* is just simply a data structure to hold two counters objects.  

### Testing plan

*Unit testing*.  A program *indextest* will serve as a unit test for the *index* module; it reads an index file into the internal *index* data structure, then writes the index out to a new index file.

*Integration testing*.  The *indexer*, as a complete program, will be tested by building an index from a pageDirectory, and then the resulting index will be validated by running it through the *indextest* to ensure it can be loaded.

1. Test `querier` with various invalid arguments and inputs.
    2. no arguments
    3. one argument
    4. three or more arguments
    5. invalid `pageDirectory` (non-existent path)
    6. invalid `indexFilename` (non-existent path)
    7. bad query syntax
    8. bad query syntax
0. Run *querier* on a medium sized pageDirectory, and indexFile
0. Run *valgrind* on *querier* to ensure no memory leaks or errors.