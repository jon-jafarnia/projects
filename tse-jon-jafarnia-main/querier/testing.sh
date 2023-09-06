## Jon Jafarnia Testing File
## No arguments
./querier

## One argument
./querier test_arg

## 3+ arguments
./querier test_arg1 test_arg2 test_arg3

## Invalid page_directory
./querier bad_dir ../indexer/index_file4

## Invalid index_Filename directory
./querier /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-depth-5  bad_index_file

# Bad syntax in query
echo or first | ./querier /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-depth-5 ../indexer/index_file4

# Bad syntax in query
echo first and | ./querier /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-depth-5 ../indexer/index_file4

# Working properly
echo breadth first or home and depth | ./querier /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-depth-5 ../indexer/index_file4

# Working properly with valgrind
echo breadth first or home and depth | valgrind ./querier /thayerfs/courses/22fall/cosc050/workspace/tse/tse-output/letters-depth-5 ../indexer/index_file4
