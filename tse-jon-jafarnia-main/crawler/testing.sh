##Jon Jafarnia Testing File
##Incorrrect Number of Args
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../directories  1 extra

##Out of depth range
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../directories  12

##External URL as seed URL
./crawler http://en.wikipedia.org/wiki/Breadth-first_search ../directories  10

##Invalid page directory
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html invalid_directory  12

#Working with letters at depth 2
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../directories  2

#Working with letters at depth 10
./crawler http://cs50tse.cs.dartmouth.edu/tse/letters/index.html ../directories  10