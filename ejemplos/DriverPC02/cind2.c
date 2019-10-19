/*Vamos a programar un char device medio*/

/*
Para compilar el driver hacemos uso del makefile simplemente con el comando "make"

	make 

Luego usamos (como root) las funciones de insmode y rmmode para instalar y remover el driver

	sudo insmode cind.ko
	sudo rmmode cind.ko

Para ver el log se usa dmesg y para borrar el mismo debemos hacer sudo dmesg -c

	dmesg
	sudo dmesg -c

Para comprobar que estan instalados en el sistema, hacemos:

	ls -la /sys/class | grep chardrv 					(DRIVER_CLASS)
	ls -la /dev | grep Dan 								(DRIVER_NAME_SHORT)

Para comprobar el buen funcionamiento del driver podemos hacer un programa de prueba que compilamos con:

	gcc test_write.c -o test_write
	gcc test_read.c -o test_read

Para ejecutar las pruebas vamos a hacer la ejecucion de los programas en modo sudo:

	sudo ./test_write
	sudo ./test_read
*/

#include <asm-generic/errno.h>
#include <asm-generic/errno-base.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

#define DRIVER_CLASS			"chardrv"
#define DRIVER_NAME 			"Dante"
#define DRIVER_NAME_SHORT		"Dan"
#define DRIVER_MINORBASE		0
#define DRIVER_MINORCOUNT		1

#define DRIVER_DEV_DATA			NULL
#define DRIVER_DEV_PARENT		NULL

static dev_t driver_dev;
static struct class *driver_dev_class;
static struct cdev driver_dev_char; 
static char driver_data_k[4] = {0};

int driver_open (struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Controlador Abierto \n");
	return 0;
}

int driver_close (struct inode *inode, struct file *filp)
{
	printk(KERN_INFO "Controlador Cerrado \n");
	return 0;
}

ssize_t driver_read (struct file *filep, char *buff, size_t count, loff_t *offp)
{
	ssize_t status = 0;

	printk (KERN_INFO "Controlador Leyendo... \n");

	if(count > MAX_RW_COUNT)
	{
		count = MAX_RW_COUNT;
	}

	if( unlikely ( ! access_ok( buff, count ) ) ) //VERIFY_WRITE
	{
		printk (KERN_ERR "El buffer es invalido \n");
		status = -EACCES;
	}
	else
	{
		if((status = copy_to_user(buff, driver_data_k, count)) != 0 )
		{
			printk (KERN_ERR "No es posible copiar en el buffer \n");
		}
	}

	printk(KERN_INFO "Controlador Leido \n");

	return 0;
}

ssize_t driver_write (struct file *filep, const char *buff, size_t count, loff_t *offp)
{
	ssize_t status = 0;

	printk(KERN_INFO "Escribiendo Controlador...\n");

	if(count > MAX_RW_COUNT)
	{
		count = MAX_RW_COUNT;
	}

	if( unlikely ( ! access_ok( buff, count ) ) ) //VERIFY_WRITE
	{
		printk (KERN_ERR "El buffer es invalido \n");
		status = -EACCES;
	}
	else
	{
		if((status = copy_from_user(driver_data_k, buff, count)) != 0 )
		{
			printk (KERN_ERR "No es posible copiar en el buffer \n");
		}
	}

	printk(KERN_INFO "Controlador Escrito \n");

	return count;
}

struct file_operations driver_dev_fops = {
	.owner 	 = THIS_MODULE,
	.open 	 = driver_open,
	.release = driver_close,
	.read    = driver_read,
	.write 	 = driver_write
};
		
static int __init cind_init(void)
{
	int status = 0;

	if(( status = alloc_chrdev_region (&driver_dev, DRIVER_MINORBASE, DRIVER_MINORCOUNT, DRIVER_NAME)) < 0 )
	{
		printk(KERN_ERR "error al pedir memoria\n");
		return(status);
	}
	if(( driver_dev_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL )
	{
		unregister_chrdev_region(driver_dev, DRIVER_MINORCOUNT);
		return(EFAULT);
	}
	if( device_create(driver_dev_class, DRIVER_DEV_PARENT, driver_dev, DRIVER_DEV_DATA, DRIVER_NAME_SHORT) == NULL )
	{
		class_destroy(driver_dev_class);
		return(EFAULT);
	}

	cdev_init(&driver_dev_char, &driver_dev_fops);

	if( (status = cdev_add(&driver_dev_char, driver_dev, DRIVER_MINORCOUNT) ) < 0 )
	{
		device_destroy(driver_dev_class, driver_dev);
		class_destroy(driver_dev_class);
		unregister_chrdev_region(driver_dev, DRIVER_MINORCOUNT);
		return(status);
	}

	printk(KERN_ERR "Controlador medio registrado\n");

	return 0;
}

static void __exit cind_exit(void)
{

	cdev_del(&driver_dev_char);
	device_destroy(driver_dev_class, driver_dev);
	class_destroy(driver_dev_class);
	unregister_chrdev_region(driver_dev, DRIVER_MINORCOUNT);

	printk(KERN_ERR "Controlador medio desregistrado\n");
}

module_init(cind_init);
module_exit(cind_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dante");
MODULE_DESCRIPTION("Controlador Medio");