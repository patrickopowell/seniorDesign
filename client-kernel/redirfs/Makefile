ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

.PHONY: build clean

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c modules.order Module.symvers
else

$(info Building with KERNELRELEASE = ${KERNELRELEASE})
obj-m += redirfs.o
redirfs-objs := rfs_path.o rfs_root.o rfs_info.o rfs_file.o rfs_dentry.o \
	rfs_inode.o rfs_dcache.o rfs_chain.o rfs_ops.o rfs_data.o \
	rfs_flt.o rfs_sysfs.o rfs.o

endif
