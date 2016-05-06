#!/bin/bash
# Log into each client and start their test scripts.
# @author Remington Campbell <racampbe@ncsu.edu
printf 'Starting testing...\n'
if [ $# -lt 4 ]; then
	printf 'start-test.sh NUM_CLIENTS MOUNT_ID QQSERVER_IP QQSTORAGE_ID\n'
	exit 1
fi
for i in `seq 0 1 $1`
do
	vagrant ssh client$i -c "nohup bash /vagrant/tests/test.sh $i $2 $3 $4 > client$i.log &"
done
printf 'All client are now testing. Await results in tests/results folder.\n'