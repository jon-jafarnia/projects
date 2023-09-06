/* 
 * pagedir.h - header file for CS50 pagedir module
//  * This file essentially works to allow us to intialize and save pages.
//  * The file checks to make sure it is given a valid directory and that it can write to it
//  * The save function works by writing a webpage to a new file. It saves a webpages url, depth, and HTML
//  * Very important that this file frees the memory it works with so that the user doesn't have to worry about it.
//  * 
//  * Jon Jafarnia, CS 50, Fall 2022
 */

#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pagedir.h"
#include "webpage.h"
#include "file.h"

/**************** functions ****************/

/**************** pagedir_init ****************/
/* Boolean that returns true if a valid directory was given and the user can write files to it
 *
 * We return:
 *   true if we can write in the directory; false if error. 
 * We guarantee:
 *   No memory leaks from this, and if returns true then the user can write to the file.
 */
bool pagedir_init(const char* pageDirectory);

// /**************** pagedir_save ****************/
// /* Save a webpage to a new file inside a given directory.
//  * 
//  * Caller provides:
//  *   valid pointer to a webpage, valid pointer to a directory, and an integer docID 
//  *   (must be a unique docID or you will write over a previous file)
//  * We return:
//  *   nothing, but program exits if there was an error saving
//  * We guarantee:
//  *   The page will be saved if valid directory, webpage, and docID.
//  * We do:
//  *  allocate memory for the path
//  *  Create/open a file at the given path
//  *  Save the page to this file with url on first line, depth on second line, and the page's html below it
//  *  Frees all memory it allocates
//  */
void pagedir_save(const webpage_t* page, const char* pageDirectory, const int docID);

/**************** pagedir_load ****************/
/* Boolean that returns true if a valid directory was given and the user can write files to it
 *
 * We return:
 *   index
 * We guarantee:
 *   loads a index
 */
webpage_t* pagedir_load(char* pageDirectory, int docID);

/**************** pagedir_validate ****************/
/* Boolean that returns true if a valid directory was given and the user can write files to it
 *
 * We return:
 *   true if valid
 * We guarantee:
 *   work
 */
bool pagedir_validate(char* pageDirectory);

#endif // __PAGEDIR_H