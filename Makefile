
ifneq ($(KERNELRELEASE),)
# kbuild part of Makefile
obj-m   := hello1.o hello2.o
ccflags-y := -I$(M)/inc
else
# Normal Makefile
KDIR ?= /lib/modules/$(shell uname -r)/build

default:
	$(MAKE) -C $(KDIR) M=$(PWD)

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
endif

