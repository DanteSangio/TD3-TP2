NAME = td3_spi
MOD_NAME = src/td3_spi
PASSWORD = temppwd
IP = 192.168.7.2

#CC = /home/dante/Imagen_BBB/gcc-linaro-6.4.1-2018.05-x86_64_arm-linux-gnueabihf/bin/arm-linux-gnueabihf-gcc

KERNEL_SOURCE := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)
INCLUDES := $(shell pwd)/inc

obj-m += $(MOD_NAME).o
#$(MOD_NAME)-objs := $(MOD_NAME)_cdev.o $(MOD_NAME)_globals.o $(MOD_NAME)_pdev.o



.PHONY: all
all: rm_mod compile
	

.PHONY: rm_mod
rm_mod:
	sudo dmesg -c
	sudo rmmod ${NAME}
#	sudo rmmod ${MOD_NAME}.ko


.PHONY: install
install:
	sudo insmod ${MOD_NAME}.ko
	ls -l /dev/ | grep spi
	lsmod | grep td3_spi
	dmesg
	@sudo chmod 777 /dev/td3_spi


.PHONY: compile
compile:
	${MAKE} -C ${KERNEL_SOURCE} KBUILD_EXTMOD=${PWD} -I${INCLUDES} modules
	sudo insmod ${MOD_NAME}.ko
	ls -l /dev/ | grep spi
	lsmod | grep td3_spi
	dmesg
	sudo dmesg -C
	@sudo chmod 777 /dev/td3_spi


.PHONY: test
test:
	./test/td3_spi_test


.PHONY: monitor
monitor:
	udevadm trigger
	udevadm monitor --env


.PHONY: clean_mod
clean_mod:
	rm *.ko *.o Module.symvers modules.order *.mod.c
	${MAKE} -C ${KERNEL_SOURCE} SUBDIRS=${PWD} *.c clean



