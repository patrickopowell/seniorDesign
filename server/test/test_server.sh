#!/bin/sh
##########################################################
# To run tests, open terminal and navigate to this folder.
# type: "sh test_server.sh"
##########################################################
cd ../src
gcc -Wall -std=c99 -g ../test/qqserver_test.c interface.c Client.c data_structures.c monitor.c Parser.c -lcunit -pthread -lgcov -o ../test/interface


