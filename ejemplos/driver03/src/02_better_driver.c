/** 
   \file    02_better_driver.c   
   \version 01.00
   \brief   Driver basico de Linux
            - caracteristica 1
            - caracteristica 2

   \author  Agustin Diaz Antuna
   \date    23/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
#include "02_better_driver.h"


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

   para ver si esta instalado:
      ls -la /dev | grep mi_drv
      ls -la /sys/class | grep midriver_class
      cat /proc/devices


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


/* ------------------------------------------------------------------------------
 * --- struct
 * ------------------------------------------------------------------------------
 */
struct file_operations cind_dev_fops =
{
   .owner = THIS_MODULE,
   .open = cind_open,
   .read = cind_read,
   .write = cind_write,
   .release = cind_close
};


/* ------------------------------------------------------------------------------
 * --- variables
 * ------------------------------------------------------------------------------
 */
static dev_t cind_dev;
static struct class *cind_dev_class;
static struct cdev cind_dev_char;
static char   message[4] = {0};           ///< Memory for the string that is passed from userspace
static short  size_of_message;              ///< Used to remember the size of the string stored


/* ------------------------------------------------------------------------------
 * --- functions
 * ------------------------------------------------------------------------------
 */
// -----> read
ssize_t cind_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
   int error_count = 0;

   printk(KERN_INFO "=====================================================");
   printk(KERN_INFO "===> midriver : Leyendo\n");
   printk(KERN_INFO "=====================================================");

   // copy_to_user has the format ( * to, *from, size) and returns 0 on success
   error_count = copy_to_user(buffer, message, size_of_message);

   if (error_count==0){            // if true then have success
      printk(KERN_INFO "EBBChar: Sent %d characters to the user\n", size_of_message);
      return (size_of_message=0);  // clear the position to the start and return 0
   }
   else {
      printk(KERN_INFO "EBBChar: Failed to send %d characters to the user\n", error_count);
      return -EFAULT;              // Failed -- return a bad address message (i.e. -14)
   }
}


// -----> write
static ssize_t cind_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
   int i = 0;

   printk(KERN_INFO "=====================================================");
   printk(KERN_INFO "===> midriver : Escribiendo\n");
   printk(KERN_INFO "=====================================================");

   
   message[0] = 'a';
   message[1] = 'a';
   message[2] = 'a';
   message[3] = '\0';

   strcpy(message, buffer);
//   size_of_message = strlen(message);                 // store the length of the stored message
//   printk(KERN_INFO "EBBChar: Received %zu characters from the user\n", len);

   return len;
}


// -----> open
int cind_open(struct inode *inode, struct file *filp)
{
   printk(KERN_INFO "=====================================================");
   printk(KERN_INFO "===> midriver : Abierto\n");
   printk(KERN_INFO "=====================================================");

   return 0;
}


// -----> close
int cind_close(struct inode *inode, struct file *filp)
{
   printk(KERN_INFO "=====================================================");
   printk(KERN_INFO "===> midriver : Cerrado\n");
   printk(KERN_INFO "=====================================================");

   return 0;
}


// -----> constructor
// init: NO alocar recursos que va a necesitar READ, eso lo hace OPEN todo 
// static para que no tenga conflicto con otros nombres
static int __init cind_init(void)
{
   int status = 0;


   printk(KERN_INFO "=====================================================");
   printk(KERN_INFO "===> midriver : Registrando\n");
   printk(KERN_INFO "=====================================================");


   // creo region
   if((status = alloc_chrdev_region(&cind_dev, CIND_MINORBASE, CIND_MINORCOUNT, CIND_NAME)) < 0)
   {
      printk(KERN_ERR "=====================================================");
      printk(KERN_ERR "===> %d (%s %d)\n", status, __FUNCTION__, __LINE__);
      printk(KERN_ERR "=====================================================");

      return (status);
   }


   // creo clase
   if((cind_dev_class = class_create(THIS_MODULE, CIND_CLASS)) == NULL)
   {
      unregister_chrdev_region(cind_dev, CIND_MINORCOUNT);

      return EFAULT;
   }


   // asocio region con clase
   if((device_create(cind_dev_class, CIND_DEV_PARENT, cind_dev, CIND_DEV_DATA, CIND_NAME_SHORT)) == NULL)
   {
      class_destroy(cind_dev_class);
      unregister_chrdev_region(cind_dev, CIND_MINORCOUNT);

      return EFAULT;
   }


   // inicializo el device (tipo, file operations)
   cdev_init(&cind_dev_char, &cind_dev_fops);

   if((status  = cdev_add(&cind_dev_char, cind_dev, CIND_MINORCOUNT)) < 0)
   {
      device_destroy(cind_dev_class, cind_dev);
      class_destroy(cind_dev_class);
      unregister_chrdev_region(cind_dev, CIND_MINORCOUNT);

      return (status);
   }


   printk(KERN_INFO "=====================================================");
   printk(KERN_INFO "===> midriver : Registrado con <Major,Minor>: %d,%d\n", MAJOR(cind_dev), MINOR(cind_dev));
   printk(KERN_INFO "=====================================================");


   return (status);
}


// -----> destructor
// destructor
static void __exit cind_exit(void)
{
   printk(KERN_INFO "=====================================================");
   printk(KERN_INFO "===> midriver : Desregistrando\n");
   printk(KERN_INFO "=====================================================");


   cdev_del(&cind_dev_char);
   device_destroy(cind_dev_class, cind_dev);
   class_destroy(cind_dev_class);
   unregister_chrdev_region(cind_dev, CIND_MINORCOUNT);


   printk(KERN_INFO "=====================================================");
   printk(KERN_INFO "===> midriver : Desregistrado\n");
   printk(KERN_INFO "=====================================================");
}


// Asociamos nuestras funciones
module_init(cind_init);
module_exit(cind_exit);


// Macros para compatibilidad que nos pide Linux
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DiazAntuna");
MODULE_DESCRIPTION("Driver de ejemplo");







