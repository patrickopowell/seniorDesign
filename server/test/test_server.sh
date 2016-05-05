#!/bin/sh
##########################################################
# To run tests, open terminal and navigate to this folder.
# type: "sh test_server.sh"
##########################################################
cd ../src
gcc -Wall -std=c99 -g interface.c Client.c data_structures.c monitor.c Parser.c -o interface.out
mv interface.out ../test/
