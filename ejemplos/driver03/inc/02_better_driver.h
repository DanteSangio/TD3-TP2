/** 
   \file    02_better_driver.h   
   \version 01.00
   \brief   Driver basico de Linux : header
            - caracteristica 1
            - caracteristica 2

   \author  Agustin Diaz Antuna
   \date    23/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
#include <asm-generic/errno.h>
#include <asm-generic/errno-base.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/version.h>
#include <linux/uaccess.h>
#include <linux/string.h>


/* ------------------------------------------------------------------------------
 * --- define
 * ------------------------------------------------------------------------------
 */
#define CIND_CLASS         "midriver_class"
#define CIND_MINORBASE     0
#define CIND_MINORCOUNT    1
#define CIND_NAME          "midriver"
#define CIND_NAME_SHORT    "mi_drv"

#define CIND_DEV_PARENT    NULL
#define CIND_DEV_DATA      NULL


/* ------------------------------------------------------------------------------
 * --- prototipos
 * ------------------------------------------------------------------------------
 */
ssize_t cind_read(struct file *filep, char *buffer, size_t len, loff_t *offset);
static ssize_t cind_write(struct file *filep, const char *buffer, size_t len, loff_t *offset);
int cind_open(struct inode *inode, struct file *filp);
int cind_close(struct inode *inode, struct file *filp);







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