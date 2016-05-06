#!/bin/bash
# Build and install QQClient into the /usr/bin/ directory.
# Accessible by client shell without qualified path specification.
# @author Remington Campbell <racampbe@ncsu.edu
cd src
make
cd ../bin
cp -f qqclient /usr/bin/qqclient
printf "QQClient installed to /usr/bin/qqclient\n"