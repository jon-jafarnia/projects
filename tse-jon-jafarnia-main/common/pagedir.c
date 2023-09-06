/* 
 * pagedir.c - CS50 'pagedir' module
 *
 * see pagedir.h for more information.
 *
 * Jon Jafarnia, October 2022
 * CS 50, Fall 2022
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"

/**************** pagedir_init() ****************/
/* see pagedir.h for description */
bool pagedir_init(const char* pageDirectory)
{
    // construct the pathname for the .crawler file in that directory
    char* l = "/.crawler";
    char* path = malloc(strlen(pageDirectory)+ strlen(l) + 1); //strlength
    sprintf(path, "%s/.crawler", pageDirectory);

    // open the file for writing; on error, return false.
    FILE* fp;
    fp = fopen(path, "w");
    if (fp == NULL){
        printf("\n\n****File not opened for writing INIT****\n\n");
        free(path);
        return false;
    }
    // close the file and return true.
    if (fclose(fp) != 0){
        free(path);
        printf("\n\n****File not closed INIT****\n\n");
        return false;
    }
    free(path);
    return true;
}

/**************** pagedir_save() ****************/
/* see pagedir.h for description */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID)
{
    // construct the pathname for the page file in pageDirectory
    char* l = "../";
    char* path = malloc(strlen(pageDirectory)+ strlen(l) + sizeof(docID) + 1);
    sprintf(path, "%s/%d", pageDirectory, docID);
    // open that file for writing
    FILE* fp;
    fp = fopen(path, "w");
    if (fp == NULL){
        free(path);
        printf("\n\n****File not opened for writing SAVE****\n\n");
        exit(1);
    }
    // print the URL
    char* url = webpage_getURL(page);
    fprintf(fp, "%s\n", url);
    // print the depth
    fprintf(fp, "%d\n", webpage_getDepth(page));
    // print the contents of the webpage
    fprintf(fp, "%s\n", webpage_getHTML(page));
    // close the file
    if (fclose(fp) != 0){
        printf("\n\n****File not closed SAVE****\n\n");
        free(path);
        exit(2);
    }
    free(path);
}

webpage_t* pagedir_load(char* pageDirectory, int docID){
    //create the path
    char* path = malloc(strlen(pageDirectory) + sizeof(docID) + 1);
    sprintf(path, "%s/%d", pageDirectory, docID);
    //check path
    if(path == NULL){
        fprintf(stderr, "Invalid path\n");
        exit(3);
    }
    FILE* fp;
    fp = fopen(path, "r");
    if(fp == NULL){
        free(path);
        return NULL;
    }
    //read the url and depth from first 2 lines
    char *url = file_readLine(fp);
    char *depth = file_readLine(fp);
    //convert to integer
    int integer_of_depth = atoi(depth);
    //make webpage
    webpage_t* page = webpage_new(url, integer_of_depth, NULL);
    //fetch html
    if(webpage_fetch(page) == false){
        fprintf(stderr, "Failed to fetch html\n");
        exit(5);
    }
    //close and free memory
    fclose(fp);
    free(path);
    free(depth);
    //return page
    return page;
}

bool pagedir_validate(char* pageDirectory){
    // try to open pagedir/.crawler('r')
    char crawler_file[1000];
    //copy strings into crawlerfile
    strcpy(crawler_file, pageDirectory);
    strcat(crawler_file, "/.crawler");
    //print where we get so that the user knows which file is being searched for
    fprintf(stdout, "%s\n", crawler_file);
    //return true if file exists
    FILE *fp = fopen(crawler_file, "r");
    if (fp != NULL){
        fclose(fp);
        return true;
    }
    //false if doesn't exist
    return false;
}