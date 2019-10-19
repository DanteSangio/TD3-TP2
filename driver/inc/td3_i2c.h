/** 
   \file    td3_i2c.h   
   \version 01.00
   \brief   General header file of td3_i2c device driver.
            - linux includes
            - prototipos

   \author  Agustin Diaz Antuna
   \date    23/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */

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




/* ------------------------------------------------------------------------------
 * --- define
 * ------------------------------------------------------------------------------
 */

#define CLASS        "td3_i2c_class"
#define MINORBASE    0
#define MINORCOUNT   1
#define NAME         "td3_i2c"
#define NAME_SHORT   "td3_i2c"

#define DEV_PARENT   NULL
#define DEV_DATA     NULL

// ioctl
#define IOCTL_CANT   1




/* ------------------------------------------------------------------------------
 * --- prototipos
 * ------------------------------------------------------------------------------
 */

// file operations
static ssize_t td3_i2c_read(struct file *filep, char *buffer, size_t len, loff_t *offset);
static ssize_t td3_i2c_write(struct file *filep, const char *buffer, size_t len, loff_t *offset);
static int td3_i2c_open(struct inode *inode, struct file *filp);
static int td3_i2c_close(struct inode *inode, struct file *filp);
static long td3_i2c_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

// plataform device
static int td3_i2c_probe(struct platform_device *pdev);
static int td3_i2c_remove(struct platform_device *pdev);

// irq handler
irqreturn_t td3_i2c_handler(int irq, void *dev_id, struct pt_regs *regs);


// MPU6050 functions
void initMPU6050(void);

// private functions
void td3_i2c_writebyte(uint8_t data);
uint8_t td3_i2c_readbyte(void);

// debug
void td3_i2c_register_debug(void);



/*
   Barrera de memoria!!!

   // inicializacion
   static atomic_t cind_open_once = ATOMIC_INIT(1);
   atomic_dec_and_test(&cind_open_once);

   // en el close
   atomic_set(&cind_open_once, 1);




   Preguntas tipicas:

   Como sabe el insmod que metodo llamar? Modificando la estructura de file_operations
   Quien checkea que el espacio donde el usuario quiere que le devuelvan lo leido por read() sea valido? ----
*/



