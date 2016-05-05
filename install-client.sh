#!/bin/bash
cd qqclient
printf 'Running qqclient install script!\n'
bash install.sh
cd ..
cd qqfs
echo 'Running qqfs install script!\n'
bash install.sh
cd ..