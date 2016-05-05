#!/bin/bash
cd qqclient
printf 'Running qqclient install script!\n'
bash qqclient-install.sh
cd ..
cd qqfs
echo 'Running qqfs install script!\n'
bash qqfs-install.sh
cd ..