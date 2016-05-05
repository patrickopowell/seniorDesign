#!/bin/bash
cd ~
mkdir test_base_dir
mkdir test_export_dir
mkdir test_misc_dir
mount $1 test_dir
qqclient ~/test_base_dir ~/test_export_dir/ $2 1 &
cd test_export_dir
fname=client$3.dat
for i in `seq 1 1 20`
do
	dd if=/dev/zero of=$fname bs=1M count=$(($RANDOM % 1024))
	sleep 2
	\cp -f $fname ../test_misc_dir/
done
killall -SIGINT qqclient
sleep 3
umount test_base_dir
rm -rf test_misc_dir
rm -rf test_export_dir
rm -rf test_base_dir