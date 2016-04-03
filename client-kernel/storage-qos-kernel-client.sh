cd /home/popowell/2016springTeam28/client-kernel/redirfs/

make clean

make

sudo make -C /lib/modules/`uname -r`/build M=`pwd` modules_install

sudo depmod -a

sudo insmod ./redirfs.ko

cd /home/popowell/2016springTeam28/client-kernel/

make clean

cp /home/popowell/2016springTeam28/client-kernel/redirfs/Module.symvers .

make

sudo make -C /lib/modules/`uname -r`/build M=`pwd` EXTRA_CFLAGS=-I/home/popowell/2016springTeam28/client-kernel/redirfs modules_install

sudo insmod ./storage-qos-kernel-client.ko