MOD_NAME = cind3
MOD_SHORT_NAME = Dan

ifneq (${KERNELRELEASE},)
	obj-m := ${MOD_NAME}.o
else
	KERNEL_SOURCE := /usr/src/linux-headers-5.0.0-31-generic/
	PWD := $(shell pwd)

compile_mod:
	${MAKE} -C ${KERNEL_SOURCE} KBUILD_EXTMOD=${PWD} modules

clean_mod:
	${MAKE} -C ${KERNEL_SOURCE} KBUILD_EXTMOD=${PWD} clean

install_mod:
	sudo insmod ${MOD_NAME}.ko

monitor_mod:
	udevadm trigger
	udevadm monitor --env

list_mods:
	lsmod | grep ${MOD_NAME}
	ls -la /dev | grep ${MOD_SHORT_NAME}

debug_mod:
	dmesg

clean_dmseg:
	sudo dmesg -c
	
rm_mod:
	rm *.ko *.o Module.symvers modules.order *.mod.c
	sudo rmmod ${MOD_NAME}.ko

endif
