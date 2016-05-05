#!/bin/bash
cd src
make
cd ../bin
cp -f qqclient /usr/bin/qqclient
printf "QQClient installed to /usr/bin/qqclient\n"