#Youngjoo Lee, Team15
#CS50, 11/10/2022

# Test the program with various forms of incorrect command-line arguments to ensure that its command-line parsing, and validation of those parameters, works correctly.
./dealer15 5 8095 5 5 
#incorrect arguments
./dealer15 0 8095
./dealer15 1 800000000000
# Play many thousands of games.
./dealer15 1000 8095
