ifneq (${KERNELRELEASE},)
	obj-m := cindBBBv2.o
else
	KERNEL_SOURCE := /usr/src/linux-headers-5.0.0-27-generic/
	PWD := $(shell pwd)
default:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} modules

clean:
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} clean
endif