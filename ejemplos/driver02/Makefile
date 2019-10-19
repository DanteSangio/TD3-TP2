MOD_NAME = 02_better_driver

ifneq (${KERNELRELEASE},)
	obj-m := ${MOD_NAME}.o
else
	KERNEL_SOURCE := /usr/src/linux-headers-$(shell uname -r)
	PWD := $(shell pwd)

compile_mod:
	${MAKE} -C ${KERNEL_SOURCE} KBUILD_EXTMOD=${PWD} ${MOD_NAME}.c modules

clean_mod:
	${MAKE} -C ${KERNEL_SOURCE} KBUILD_EXTMOD=${PWD} ${MOD_NAME}.c clean

install_mod:
	sudo insmod ${MOD_NAME}.ko

monitor_mod:
	udevadm trigger
	udevadm monitor --env

list_mods:
	lsmod | grep ${MOD_NAME}
	ls -l /dev/${MOD_NAME}*

debug_mod:
	dmesg

clean_dmseg:
	sudo dmesg -c
	
rm_mod:
	rm *.ko *.o Module.symvers modules.order *.mod.c
	sudo rmmod ${MOD_NAME}.ko
endif
