#!/bin/bash
cd src
make
cd ../bin
cp qqclient /usr/bin/qqclient
printf "QQClient installed to /usr/bin/qqclient\n"