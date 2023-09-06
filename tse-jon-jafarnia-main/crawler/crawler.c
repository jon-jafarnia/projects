/* 
 * crawler.c - a C program that crawls webpages to a given depth and saves all fo the valid pages it collects
 *
 * We will eventually use the crawler for our tiny search engine. 
 * It plays a vital role in collecing all of the files needed for our engine.
 * 
 * input: A seed url, a valid directory, and a valid depth
 * output: A bunch of files saved in the given directory
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
#include "../common/pagedir.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#include "../libcs50/bag.h"

//Main function, calls parse args and crawl then returns 0
// should be called ./crawler seedurl, pagedir, maxdepth
int main(const int argc, char* argv[]);

//Given arguments from the command line, extract them into the 
//function parameters; return only if successful.
static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth);

//Do the real work of crawling from seedURL to maxDepth and saving pages
// in pageDirectory
static void crawl(char* seedURL, char* pageDirectory, const int maxDepth);

//This function implements the pagescanner mentioned in the design. 
//Given a webpage, scan the given page to extract any links (URLs), 
//ignoring non-internal URLs; for any URL not already seen before 
//(i.e., not in the hashtable), add the URL to both the hashtable pages_seen
// and to the bag pages_to_crawl
static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen);

int main(const int argc, char* argv[]){
    //check number of arguments
    if (argc != 4){
        printf("Invalid number of arguments\n");
        return 1;
    }
    //set up variables and pointers
    int maxDepth = 0;
    char* seedURL = NULL;
    char* pageDirectory;
    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawl(seedURL, pageDirectory, maxDepth);
}

static void parseArgs(const int argc, char* argv[],
                      char** seedURL, char** pageDirectory, int* maxDepth){
                        //make sure correct number of arguments
                        if (argc != 4){
                            printf("Invalid number of arguments\n");
                            exit(1);
                        }
                        //normalize seed url
                        //ALERT this part is leaking memory
                        *seedURL = argv[1];
                        char* temp = normalizeURL(*seedURL);
                        if ( temp == NULL){
                            printf("URL cannot be normalized\n");
                            exit(2);
                        }
                        free(temp);
                        //make sure seedurl is internal
                        if (!isInternalURL(*seedURL)){
                            printf("Not internal URL\n");
                            exit(2);
                        }
                        //make maxdepth an integer
                        char* max_depth = argv[3];
                        char excess;
                        if (sscanf(max_depth, "%d%c", maxDepth, &excess) != 1){
                            printf("Invalid depth\n");
                            exit(1);
                        }
                        //check page directory
                        *pageDirectory = argv[2];
                        if (pagedir_init(*pageDirectory) == false){
                            printf("Invalid page directory\n");
                            exit(3);
                        }
                        //check depth is valid
                        if (*maxDepth < 0 || *maxDepth > 10){
                            printf("Out of depth range. Must be in range [0...10]\n");
                            exit(4);
                        }
                      }

static void crawl(char* seedURL, char* pageDirectory, const int maxDepth){
    // initialize the hashtable and add the seedURL
    hashtable_t* page_table = NULL;
    page_table = hashtable_new(200);
    hashtable_insert(page_table, seedURL, "n/a");
    // initialize the bag and add a webpage representing the seedURL at depth 0
    bag_t* page_bag = NULL;
    page_bag = bag_new();
    char* url = malloc(strlen(seedURL)+1);
    strcpy(url, seedURL);
    printf("%s\n", url);
    webpage_t* page = webpage_new(url, 0, NULL);
    bag_insert(page_bag, page);

    //make a weboage to hold the current page
    webpage_t* curr_page;
    
    int docID = 1;
    // while bag is not empty
    //  pull a webpage from the bag
    while((curr_page = bag_extract(page_bag)) != NULL){
    //  fetch the HTML for that webpage and make sure it works
    //  if fetch was successful,
        if (webpage_fetch(curr_page)){
    //  save the webpage to pageDirectory
            pagedir_save(curr_page, pageDirectory, docID);
            docID += 1;
            fprintf(stdout, "%d Fetched: %s\n", webpage_getDepth(curr_page), webpage_getURL(curr_page));
                //     if the webpage is not at maxDepth,
            if (webpage_getDepth(curr_page) < maxDepth){
                //         pageScan that HTML
                pageScan(curr_page, page_bag, page_table);
            }
        }
        else{
            //Print failed fetch
            fprintf(stderr, "%d FailFetch: %s\n", webpage_getDepth(curr_page), webpage_getURL(curr_page));
            continue;
        }
        webpage_delete(curr_page);
    }

    // delete the hashtable
    hashtable_delete(page_table, NULL);
    // delete the bag
    bag_delete(page_bag, NULL);
}

static void pageScan(webpage_t* page, bag_t* pagesToCrawl, hashtable_t* pagesSeen)
{
    fprintf(stdout, "%d Scanning: %s\n", webpage_getDepth(page), webpage_getURL(page));
    //set position
    int pos = 0;
    //set up new url
    char* new_url;
    // make a temp_page
    webpage_t* temp_page;
    // while there is another URL in the page
    while ((new_url = webpage_getNextURL(page, &pos)) != NULL){
        fprintf(stdout, "%d Found: %s\n", webpage_getDepth(page), new_url);
        char* temp_url = normalizeURL(new_url);
        // if that URL is Internal, whats going on
        if(isInternalURL(temp_url)){
            if(hashtable_insert(pagesSeen, temp_url, "n/a")){
                //put the new webpage into the bag and print to describe action
                temp_page = webpage_new(temp_url, webpage_getDepth(page) + 1, NULL);
                bag_insert(pagesToCrawl, temp_page);
                fprintf(stdout, "%d Added: %s\n", webpage_getDepth(page), temp_url);
            }
            else{
                //Ignoring duplicate entry to hashtable, free temp_url
                fprintf(stdout, "%d IgnDupl: %s\n", webpage_getDepth(page), temp_url);
                free(temp_url);
            }
        }
        else{
            //External url, free temp_url
            fprintf(stdout, "%d External: %s\n", webpage_getDepth(page), temp_url);
            free(temp_url);
        }
        //free memory for new url
       free(new_url);
    }
}