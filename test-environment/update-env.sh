#!/bin/bash
# Update client installations of QualiQueue components.
# Don't need to start an entirely new environment.
# @author Remington Campbell <racampbe@ncsu.edu
if [ $# -lt 1 ]; then
	printf 'start-test.sh NUM_CLIENTS\n'
	exit 1
fi
buildscript='/vagrant/autobuild/qqbuild/install-client.sh'
printf 'Starting client updates...\n'
printf "Updating from $buildscript\n"
for i in `seq 0 1 $1`
do
	vagrant ssh client$i -c "bash $buildscript"
done