ifneq (${KERNELRELEASE},)
	obj-m := 01_simple_driver.o

else
	KERNEL_SOURCE := /usr/src/linux-headers-5.0.0-27-generic/
	PWD := $(shell pwd)

default:
	${MAKE} -C ${KERNEL_SOURCE} KBUILD_EXTMOD=${PWD} modules

clean:
	${MAKE} -C ${KERNEL_SOURCE} KBUILD_EXTMOD=${PWD} clean

endif


