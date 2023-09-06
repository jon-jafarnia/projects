# Common directory
This directory will hold code used by multiple modules in the Tiny Search Engine (TSE).

## pagedir
The pagedir files holds functions pagedir_init and pagedir_save. The init function works to validate that the given directory is valid and can be written to. Furthermore, the save function writes a file by the name of docID to the given directory. The file holds a webpages url, depth, and html.

## word
The word files hold the function normalizeWord. This simply serves to make a word completely lowercase. This function is extremely helpful to the indexer.

## index
The index files hold a lot of wrapper files for hashtable. This file contains index_load and index_save which effectively serve to load and save index files. The load function works to load a previously built index from another file, and the save function works with the indexer to write a file.