/*Vamos a programar un char device basico*/

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>

static int __init cind_init(void)
{
	printk(KERN_ERR "hola\n");
	return 0;
}

static void __exit cind_exit(void)
{
	printk(KERN_ERR "chau\n");
}

module_init(cind_init);
module_exit(cind_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dante");
MODULE_DESCRIPTION("Controlador Minimo");