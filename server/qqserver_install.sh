#!/bin/bash
sudo mkdir /usr/bin/qqserver/
gcc -Wall -std=c99 -g interface.c Client.c data_structures.c monitor.c Parser.c -o interface
sudo cp -f interface /usr/bin/qqserver
printf "QQServer installed to /usr/bin/qqserver\n"





