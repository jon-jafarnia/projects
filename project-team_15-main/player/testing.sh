#!/bin/bash
#
# testing.sh - test player.c
# usage: bash testing.sh 
# Team 15
# CS 50, Fall 2022

# test with no agruments
./player15

# test with too many agruments
./player15 team_15 localhost 8095 true q.txt true load.txt 90

# test with invalid port number
./player15 team_15 localhost -5 true q.txt false

# test with invalid boolean
./player15 team_15 localhost 8095 maybe q.txt false

# test with invalid boolean combination
./player15 team_15 localhost 8095 false q.txt false

# test properly with valgrind (ensure dealer is running locally on port 8095)
valgrind ./player15 team_15 localhost 8095 true q.txt true q_100000
