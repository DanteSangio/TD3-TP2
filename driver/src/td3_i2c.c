/** 
   \file    td3_i2c.c   
   \version 01.00
   \brief   Main file of the driver td3_i2c
            Here you can find the following functions:
            - init   -> td3_i2c_init
            - exit   -> td3_i2c_exit

   \author  Agustin Diaz Antuna
   \date    23/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */

// headers
#include "../inc/td3_i2c.h"
#include "../inc/am335x.h"
#include "../inc/mpu6050.h"

// globals
#include "td3_i2c_globals.c"

// aux functions
#include "mpu6050.c"

// sources
#include "td3_i2c_auxfunctions.c"
#include "td3_i2c_pdev.c"
#include "td3_i2c_fops.c"




/* ------------------------------------------------------------------------------
 * --- stuff
 * ------------------------------------------------------------------------------
 */

// Linux's macros
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Agustin Diaz Antuna");
MODULE_DESCRIPTION("Driver i2c");




/* ------------------------------------------------------------------------------
 * --- init
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_init
 *
 * @brief      Function executed at the instalation of the module (insmod). This
 *             function must follow this steps:
 *             - Allocate char device region
 *             - Create class
 *             - Create char device
 *             - Initialize char device
 *             - Add char device
 *             - Register platform driver
 *
 * @param      void
 *
 * @return     status      error control
 *
 * ------------------------------------------------------------------------------
 */

static int __init td3_i2c_init(void)
{
   int status = 0;


   printk(KERN_INFO "|INIT| [LOG] td3_i2c : Inializating module...\n");


   // alloc char device region
   if((status = alloc_chrdev_region(&td3_i2c_dev, MINORBASE, MINORCOUNT, NAME)) < 0)
   {
      printk(KERN_ERR "|INIT| [ERROR] td3_i2c : %d (%s %d)\n", status, __FUNCTION__, __LINE__);

      return (status);
   }


   // create clase
   if((td3_i2c_class = class_create(THIS_MODULE, CLASS)) == NULL)
   {
      printk(KERN_ERR "|INIT| [ERROR] td3_i2c : (%s %d)\n", __FUNCTION__, __LINE__);

      unregister_chrdev_region(td3_i2c_dev, MINORCOUNT);

      return EFAULT;
   }


   // create device and relate region with class
   if((device_create(td3_i2c_class, DEV_PARENT, td3_i2c_dev, DEV_DATA, NAME_SHORT)) == NULL)
   {
      printk(KERN_ERR "|INIT| [ERROR] td3_i2c : (%s %d)\n", __FUNCTION__, __LINE__);

      class_destroy(td3_i2c_class);
      unregister_chrdev_region(td3_i2c_dev, MINORCOUNT);

      return EFAULT;
   }


   // initalizto the char device (type, file operations)
   cdev_init(&td3_i2c_cdev, &fops);


   // add char device
   if((status  = cdev_add(&td3_i2c_cdev, td3_i2c_dev, MINORCOUNT)) < 0)
   {
      printk(KERN_ERR "|INIT| [ERROR] td3_i2c : %d (%s %d)\n", status, __FUNCTION__, __LINE__);

      device_destroy(td3_i2c_class, td3_i2c_dev);
      class_destroy(td3_i2c_class);
      unregister_chrdev_region(td3_i2c_dev, MINORCOUNT);

      return (status);
   }


   // register platform driver to relate the device tree with the driver module
   if((status = platform_driver_register(&td3_pdriver)) < 0)
   {
      printk(KERN_ERR "|INIT| [ERROR] td3_i2c : %d (%s %d)\n", status, __FUNCTION__, __LINE__);

      cdev_del(&td3_i2c_cdev);
      device_destroy(td3_i2c_class, td3_i2c_dev);
      class_destroy(td3_i2c_class);
      unregister_chrdev_region(td3_i2c_dev, MINORCOUNT);

      return (status);      
   }


   printk(KERN_INFO "|INIT| [LOG] td3_i2c : Module registered with <Major,Minor>: %d,%d\n", MAJOR(td3_i2c_dev), MINOR(td3_i2c_dev));
   printk(KERN_INFO "|INIT| [LOG] td3_i2c : Initialization OK\n");


   return (status);
}




/* ------------------------------------------------------------------------------
 * --- exit
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_exit
 *
 * @brief      Function executed at the removal of the module (rmmod). This 
 *             function must undo all the steps performed in td3_i2c_init.
 *             - Unallocate char device region
 *             - Destroy class
 *             - Destroy char device
 *             - Uninitialize char device
 *             - Remove char device
 *             - Unregister platform driver
 *
 * @param      void
 *
 * @return     status      error control
 *
 * ------------------------------------------------------------------------------
 */

static void __exit td3_i2c_exit(void)
{
   printk(KERN_INFO "|EXIT| [LOG] td3_i2c : Exiting module...\n");


   cdev_del(&td3_i2c_cdev);
   device_destroy(td3_i2c_class, td3_i2c_dev);
   class_destroy(td3_i2c_class);
   unregister_chrdev_region(td3_i2c_dev, MINORCOUNT);
   platform_driver_unregister(&td3_pdriver);


   printk(KERN_INFO "|EXIT| [LOG] td3_i2c : Exit OK\n");
}




// Relate functions
module_init(td3_i2c_init);
module_exit(td3_i2c_exit);



