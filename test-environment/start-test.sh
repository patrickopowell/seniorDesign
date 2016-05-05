#!/bin/bash
printf 'Starting testing...'
if [ $# -lt 3 ]; then
	printf 'start-test.sh NUM_CLIENTS MOUNT_ID QQSERVER_IP\n'
for i in `seq 1 1 $1`
do
	vagrant ssh client$i -c "nohup bash /vagrant/tests/test.sh $2 $3 $i > last_test.log &"
done