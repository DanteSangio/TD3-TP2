/*Vamos a programar un char device para BBB*/


#include <asm-generic/errno.h>
#include <asm-generic/errno-base.h>

#include <linux/cdev.h>             // Char device: file operation struct, 
#include <linux/fs.h>               // Header for the Linux file system support
#include <linux/module.h>           // Core header for loading LKMs into the kernel
#include <linux/uaccess.h>          // copy_to_user - copy_from_user
#include <linux/of_address.h>       // of_iomap
#include <linux/platform_device.h>  // platform_device
#include <linux/of.h>               // of_match_ptr
#include <linux/io.h>               // ioremap
#include <linux/interrupt.h>        // request_irq
#include <linux/delay.h>            // msleep
#include <linux/types.h>            // atomic_t
#include <linux/init.h>             // Macros used to mark up functions e.g. __init __exit

// check
#include <linux/kdev_t.h>
#include <linux/device.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/of_platform.h>
#include <linux/wait.h>             // Para la queue

// #include <linux/semaphore.h>
// #include <linux/spinlock.h>
// #include <linux/list.h>
// #include <linux/string.h>

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
/*
	if(count > MAX_RW_COUNT)
	{
		count = MAX_RW_COUNT;
	}
*/
	if( unlikely ( ! access_ok( VERIFY_WRITE, buff, count ) ) ) //VERIFY_WRITE
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
/*
	if(count > MAX_RW_COUNT)
	{
		count = MAX_RW_COUNT;
	}
*/
	if( unlikely ( ! access_ok( VERIFY_WRITE, buff, count ) ) ) //VERIFY_WRITE
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

static int cindp_probe(struct platform_device *pdev)
{
	const char *aux = NULL;
	unsigned int num = 0;

	struct device_node *np;
	np = pdev->dev.of_node; 

	pr_info("Controlador minimo probando...\n");

	of_property_read_string(np, "texto-propiedad", &aux);
	of_property_read_u32(np, "valor_unico-propiedad", &num);

	printk(KERN_ERR "Se levantó la propiedad %s y %ud\n", aux, num);
	printk(KERN_ERR "Controlador minumo probado\n");

	return 0;
}

static int cindp_remove(struct platform_device *pdev)
{
	pr_info("Controlador minimo removiendo...\n");

	pr_info("Controlador minimo removido\n");

	return 0;
}

static const struct of_device_id cindp_of_match[] = {
	{ .compatible = "fabricante, modelo-modulo"},
	{},
};

static struct platform_driver cind_platform = 
{
	.driver = 	{
					.name = "cindp",
					.owner = THIS_MODULE,
					.of_match_table = of_match_ptr(cindp_of_match),
				},
	.probe = cindp_probe,
	.remove = cindp_remove,
};

struct file_operations driver_dev_fops = {
	.owner 	 = THIS_MODULE,
	.open 	 = driver_open,
	.release = driver_close,
	.read    = driver_read,
	.write 	 = driver_write
};

//__init indica que se inicializa al momento de iniciar el sistema y luego  se libera la memoria
static int __init cind_init(void)
{
	int status = 0;

	printk(KERN_INFO "|INIT| [LOG] td3_spi : Iniciando el modulo...\n");

	if(( status = alloc_chrdev_region (&driver_dev, DRIVER_MINORBASE, DRIVER_MINORCOUNT, DRIVER_NAME)) < 0 )
	{
		printk(KERN_ERR "|INIT| [ERROR] td3_spi : %d (%s %d)\n", status, __FUNCTION__, __LINE__);
		return(status);
	}
	if(( driver_dev_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL )
	{
		printk(KERN_ERR "|INIT| [ERROR] td3_spi : (%s %d)\n", __FUNCTION__, __LINE__);
		unregister_chrdev_region(driver_dev, DRIVER_MINORCOUNT);
		return(EFAULT);
	}
	if( device_create(driver_dev_class, DRIVER_DEV_PARENT, driver_dev, DRIVER_DEV_DATA, DRIVER_NAME_SHORT) == NULL )
	{
		printk(KERN_ERR "|INIT| [ERROR] td3_spi : (%s %d)\n", __FUNCTION__, __LINE__);
		class_destroy(driver_dev_class);
		unregister_chrdev_region(driver_dev, DRIVER_MINORCOUNT);
		return(EFAULT);
	}

	cdev_init(&driver_dev_char, &driver_dev_fops);

	if( (status = cdev_add(&driver_dev_char, driver_dev, DRIVER_MINORCOUNT) ) < 0 )
	{
		printk(KERN_ERR "|INIT| [ERROR] td3_spi : %d (%s %d)\n", status, __FUNCTION__, __LINE__);
		device_destroy(driver_dev_class, driver_dev);
		class_destroy(driver_dev_class);
		unregister_chrdev_region(driver_dev, DRIVER_MINORCOUNT);
		return(status);
	}

    printk(KERN_INFO "|INIT| [LOG] td3_spi : Modulo registrado con <Major,Minor>: %d,%d\n", MAJOR(driver_dev), MINOR(driver_dev));
    printk(KERN_INFO "|INIT| [LOG] td3_spi : Inicializacion OK\n");

	return 0;
}

static void __exit cind_exit(void)
{
	printk(KERN_INFO "|EXIT| [LOG] td3_spi : Saliendo del modulo...\n");

	cdev_del(&driver_dev_char);
	device_destroy(driver_dev_class, driver_dev);
	class_destroy(driver_dev_class);
	unregister_chrdev_region(driver_dev, DRIVER_MINORCOUNT);

	printk(KERN_INFO "|EXIT| [LOG] td3_spi : Salida exitosa\n");
}

module_init(cind_init);
module_exit(cind_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dante");
MODULE_DEVICE_TABLE(of, cindp_of_match);
MODULE_DESCRIPTION("Controlador Minimo");
