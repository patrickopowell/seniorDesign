ifeq ($(KERNELRELEASE),)

KERNELDIR ?= /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

.PHONY: build clean

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) EXTRA_CFLAGS=-I/home/popowell/2016springTeam28/client-kernel/redirfs modules

clean:
	rm -rf *.o *~ core .depend .*.cmd *.ko *.mod.c Module.symvers modules.order
else

$(info Building with KERNELRELEASE = ${KERNELRELEASE})
obj-m :=    storage-qos-kernel-client.o

endif
