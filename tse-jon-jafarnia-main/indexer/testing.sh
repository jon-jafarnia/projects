##Jon Jafarnia Testing File
##Incorrrect Number of Args
./indexer ../directories index_file1 extra

# Invalid Directory
./indexer ../invalid_directory index_file2

# A run with valgrind over a moderate-sized test case.
valgrind ./indexer /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/toscrape-depth-1 index_file3

# Small test case
./indexer /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-depth-5 index_file4

# Medium Test Case
./indexer /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/toscrape-depth-1 index_file5
