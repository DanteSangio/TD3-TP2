# Makefile usage:
# 	make: compiles module for BBB using a cross-compiler
#	make copy: copies the module and the test program to the BBB
# 	make clean: removes created files
#	make ssh: connects to the BBB using local IP 192.168.7.2
#
# Created by Guido Di Giovanni
# Modified by Matias Silveiro
# Last Mod by Dante Sangiovanni	

TEST_NAME = test/test_write
#TEST_NAME = test/test_read
#TEST_NAME = test/BMP280_test
#TEST_NAME = test/test_reopen

MODULE_NAME = cindBBBv3
#MODULE_NAME = i2c_mgs

obj-m := $(MODULE_NAME).o

KERNEL_DIR = /home/dante/kernel_beagle/kernel_beagle/
#KERNEL_DIR = /home/dante/Imagen_BBB/bb-kernel/KERNEL
#KERNEL_DIR = /home/matias/Escritorio/TD_III/Beaglebone/kernel_beagle

COPY_DIR = /home/debian/dante/driver/spi/
#COPY_DIR = /home/debian/TD3/i2c

BBB_USER = debian
BBB_PASS = temppwd

all:
	@echo "\033[32m>>>> Compiling modules for BBB\033[0m"
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=$(PWD) modules EXTRA_CFLAGS=-fno-pic

	@echo "\n\033[32m>>>> Compiling binary for BBB\033[0m"
	arm-linux-gnueabihf-gcc $(TEST_NAME).c -o test_prg

copy:
	@echo "\033[32m>>>> Copying files to BBB\033[0m"
	sshpass -p $(BBB_PASS) scp $(MODULE_NAME).ko $(BBB_USER)@192.168.7.2:$(COPY_DIR)
	sshpass -p $(BBB_PASS) scp test_prg	$(BBB_USER)@192.168.7.2:$(COPY_DIR)

clean:
	@echo "\033[32m>>>> Cleaning compiled files\033[0m"
	make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- -C $(KERNEL_DIR) M=$(PWD) clean
	rm -f test_prg

ssh:
	clear
	@echo "\033[32m>>>> Connecting to BBB\033[0m"
	@sshpass -p $(BBB_PASS) ssh $(BBB_USER)@192.168.7.2
