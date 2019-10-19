/*Vamos a programar un char device basico*/

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>

static int __init cind_init(void)
{
	printk(KERN_ERR "Controlador minimo registrando...\n");
	platform_driver_register(&cind_platform);
	printk(KERN_ERR "Controlador minimo registrado\n");
	return 0;
}

static void __exit cind_exit(void)
{
	printk(KERN_ERR "Controlador minimo desregistrando...\n");
	platform_driver_unregister(&cind_platform);
	printk(KERN_ERR "Controlador minimo desregistrado\n");
}

static int cind_probe(struct platform_device *pdev)
{
	pr_info("Controlador minimo probado\n");
	printk(KERN_ERR "Controlador minimo probado\n");
	return 0;
}

static int cind_remove(struct platform_device *pdev)
{
	pr_info("Controlador minimo removido\n");
	printk(KERN_ERR "Controlador minimo removido\n");
	return 0;
}

static struct platform_driver cind_platform = 
{
	.driver = 	{
					.name = KBUILD_MODNAME,
					.owner = THIS_MODULE,
				},
	.probe = cind_probe,
	.remove = cind_remove,
};

module_init(cind_init);
module_exit(cind_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dante");
MODULE_DESCRIPTION("Controlador Minimo");