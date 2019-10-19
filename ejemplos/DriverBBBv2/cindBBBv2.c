/*Vamos a programar un char device para BBB*/

#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/of_device.h>
#include <linux/of.h>

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
/*
static int mydriver_probe (struct platform_device *pdev)
{
	struct device_node *np;
	np = pdev->dev.of_node; 
	const char *aux...
}
*/

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

/*__init indica que se inicializa al momento de iniciar el sistema y luego  se libera la memoria*/
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

module_init(cind_init);
module_exit(cind_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Dante");
MODULE_DEVICE_TABLE(of, cindp_of_match);
MODULE_DESCRIPTION("Controlador Minimo");