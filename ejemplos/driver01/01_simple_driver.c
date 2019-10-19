#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>


/* Cosas especificas:
 *    Vamos a usar los drivers de forma modular
 *
 *    __init es una macro que le dice al linker que este controlador debe 
 *    ser inicializado en la rutina initmodules del kernel
 *
 *
   instalar:
      apt install linux-source
      make

   ls -l /dev


   instalamos:
      sudo insmod 01_simple_driver.ko

   desinstalamos:
      modprobe -r 01_simple_driver.ko
      sudo rmmod 01_simple_driver.ko

   para ver como se instala:
      udevadm trigger
      udevadm monitor --env


   debugeamos con:
      dmesg
      lsmod       // los modos disponibles
      lsmod | grep cm
 *
 */

// constructor
// init: NO alocar recursos que va a necesitar READ, eso lo hace OPEN
static int __init cind_init(void)
{
   printk(KERN_INFO "=====================================================");
   printk(KERN_INFO "=============================> mensaje para registrar");
   printk(KERN_INFO "=====================================================");
   return 0;
}


// destructor
static void __exit cind_exit(void)
{
   printk(KERN_INFO "=====================================================");
   printk(KERN_INFO "=========================> mensaje para des-registrar");
   printk(KERN_INFO "=====================================================");
}

// Asociamos nuestras funciones
module_init(cind_init);
module_exit(cind_exit);


// Macros para compatibilidad que nos pide Linux
MODULE_LICENSE("GPL");
MODULE_AUTHOR("FULCRUM");
MODULE_DESCRIPTION("Driver de ejemplo");


