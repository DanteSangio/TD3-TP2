/** 
   \file    td3_i2c_globals.c   
   \version 01.00
   \brief   Global variables

   \author  Agustin Diaz Antuna
   \date    30/01/2019
*/

/* ------------------------------------------------------------------------------
 * --- variables
 * ------------------------------------------------------------------------------
 */

// ---> td3_i2c.c

   // output parameter for first assigned number 
   static dev_t   td3_i2c_dev;

   // to the class created
   static struct  class *td3_i2c_class;

   // to the char device
   static struct  cdev td3_i2c_cdev;

   static const struct of_device_id i2c_of_match [] = 
   {
      { .compatible = "td3,omap4-i2c" },
      { },
   };

   // to register the platform device
   static struct platform_driver td3_pdriver =
   {
      .probe  = td3_i2c_probe,
      .remove = td3_i2c_remove,
      .driver =
      {
        .name           = "td3-i2c",
        .of_match_table = of_match_ptr(i2c_of_match),
      },
   };

   MODULE_DEVICE_TABLE(of, i2c_of_match);




// ---> td3_i2c_pdev.c
   int virq;




// ---> td3_i2c_fops.c
   static void __iomem *td3_i2c2_base, *td3_cm_per_base, *td3_control_module_base;
   uint8_t td3_i2c_data_tx = 0;
   uint8_t td3_i2c_data_rx = 0;




// queue
   volatile int td3_i2c_wk_cond = 0;
   wait_queue_head_t td3_i2c_wk = __WAIT_QUEUE_HEAD_INITIALIZER(td3_i2c_wk);

   wait_queue_head_t td3_i2c_wk_ctrlopen = __WAIT_QUEUE_HEAD_INITIALIZER(td3_i2c_wk_ctrlopen);


// ctrl open
   // https://www.kernel.org/doc/html/v4.15/core-api/atomic_ops.html
   static atomic_t td3_i2c_ctrlopen_count = ATOMIC_INIT(0);
   static atomic_t td3_i2c_wk_cond_ctrlopen = ATOMIC_INIT(0);


// init control
   uint8_t init_control = 0;


// variables with enum
   int Gscale = GFS_250DPS;
   int Ascale = AFS_2G;
   int Aaxis = AXIS_X;


// ioctl
   int read_count = 0;




