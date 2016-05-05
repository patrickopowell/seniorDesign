#!/bin/bash
if [ $# -lt 4 ]; then
	printf 'start-test.sh NUM_CLIENTS MOUNT_ID QQSERVER_IP QQSTORAGE_ID\n'
	exit 1
fi
cd ~
printf 'Creating test directories...\n'
mkdir test_base_dir
mkdir test_export_dir
mkdir test_misc_dir
printf 'Mounting test share...\n'
mount $2 test_dir
printf 'Attaching QQFS to test share...\n'
qqclient ~/test_base_dir ~/test_export_dir/ $3 $4 &
cd test_export_dir
fname=client$1.dat
printf 'Starting random read/write testing with block size 1M...\n'
for i in `seq 1 1 20`
do
	dd if=/dev/zero of=$fname bs=1M count=$(($RANDOM % 1024))
	sleep 2
	\cp -f $fname ../test_misc_dir/
done
printf 'Testing finished...\n'
printf 'Killing QQClient...\n'
killall -SIGTERM qqclient
sleep 3
printf 'Unmounting test directory...\n'
umount test_base_dir
printf 'Deleting test directories...\n'
rm -rf test_misc_dir
rm -rf test_export_dir
rm -rf test_base_dir
cp -f client$1.log /vagrant/tests/results/