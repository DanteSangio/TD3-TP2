/** 
   \file    td3_i2c_fops.c
   \version 01.00
   \brief   File operations of td3_i2c driver.
            This source file contains typical file-functions as:
            - open   -> td3_i2c_open
            - close  -> td3_i2c_close
            - read   -> td3_i2c_read
            - write  -> td3_i2c_write

   \author  Agustin Diaz Antuna
   \date    29/01/2019
*/

/* ------------------------------------------------------------------------------
 * --- struct
 * ------------------------------------------------------------------------------
 */

struct file_operations fops =
{
   .owner = THIS_MODULE,
   .open = td3_i2c_open,
   .release = td3_i2c_close,
   .read = td3_i2c_read,
   .write = td3_i2c_write,
   .unlocked_ioctl = td3_i2c_ioctl
};




/* ------------------------------------------------------------------------------
 * --- open
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_open
 *
 * @brief      File operation open of the I2C2 module driver. This function set
 *             the I2C2 registers.
 *             - I2C_CON : disable I2C2
 *             - I2C_PSC : configure prescaler for the module clock frecuency
 *             - I2C_SCLL/I2C_SCLH : set SCL frecuency
 *             - I2C_OA : set own address
 *             - I2C_SA : set slave address
 *             - I2C_CON : enable device as a master receiver
 *
 * @param      struct inode *inode
 * @param      struct file *filp
 *
 * @return     static int
 *
 * ------------------------------------------------------------------------------
 */

static int td3_i2c_open(struct inode *inode, struct file *filp)
{
   uint32_t status = 0;

   printk(KERN_INFO "|OPEN| [LOG] td3_i2c : Opening device...\n");

   // control device open
   // atomic_dec_and_test() return true when counter == 0
   if(atomic_read(&td3_i2c_ctrlopen_count) > 0)
   {
      printk(KERN_ERR "|OPEN| [ERROR] td3_i2c : DEVICE ALREADY OPEN\n");

      // wait until device is close -> td3_i2c_wk_cond_ctrlopen will value 1
      if((status = wait_event_interruptible(td3_i2c_wk_ctrlopen,
                                           atomic_dec_and_test(&td3_i2c_wk_cond_ctrlopen) == 1)) < 0)
      {
         printk(KERN_ERR "|OPEN| [ERROR] td3_i2c : open (%s %d)\n", __FUNCTION__, __LINE__);
         return -1;
      }
   }

   atomic_inc(&td3_i2c_ctrlopen_count);

   printk(KERN_INFO "|OPEN| [LOG] td3_i2c : DEVICE OPEN\n");

   /*
      21.3.15 How to Program I2C       --- pp 4488

      21.3.15.1 Module Configuration Before Enabling the Module
      1. Program the prescaler to obtain an approximately 12-MHz I2C module clock (I2C_PSC = x;
         this value is to be calculated and is dependent on the System clock frequency).
      2. Program the I2C clock to obtain 100 Kbps or 400 Kbps (SCLL = x and SCLH = x; these
         values are to be calculated and are dependent on the System clock frequency).
      3. Configure its own address (I2C_OA = x) - only in case of I2C operating mode (F/S mode).
      4. Take the I2C module out of reset (I2C_CON:I2C_EN = 1)

      21.3.15.2 Initialization Procedure
      1. Configure the I2C mode register (I2C_CON) bits.
      2. Enable interrupt masks (I2C_IRQENABLE_SET), if using interrupt for transmit/receive 
         data.
      3. Enable the DMA (I2C_BUF and I2C_DMA/RX/TX/ENABLE_SET) and program the DMA controller)
         only in case of I2C operating mode (F/S mode), if using DMA for transmit/receive data.

      21.3.15.3 Configure Slave Address and DATA Counter Registers
         In master mode, configure the slave address (I2C_SA = x) and the number of byte
         associated with the transfer (I2C_CNT = x).


      Need to configure some registers in the following order
         I2C_CON
         I2C_PSC
         I2C_SCLL
         I2C_SCLH
         I2C_OA
         I2C_SYSC
         I2C_CON
   */

   // ---> write registers
      // see the documentation on the header ---> ../inc/am335x.h
      
      // configure register -> disable I2C2
      iowrite32(0x0000, td3_i2c2_base + I2C_CON);

      // configure prescaler to 24MHz
      iowrite32(0x01, td3_i2c2_base + I2C_PSC);

      // configure SCL to 1MHz
      iowrite32(0x35, td3_i2c2_base + I2C_SCLL);
      iowrite32(0x37, td3_i2c2_base + I2C_SCLH);

      // set a random own address
      iowrite32(0x77, td3_i2c2_base + I2C_OA);

      // // configure SYSC
      // iowrite32(0x00, td3_i2c2_base + I2C_SYSC);

      // slave address -> MPU6050
      iowrite32(MPU6050_DEFAULT_ADDRESS, td3_i2c2_base + I2C_SA);

      // configure register -> ENABLE & MASTER & RX & STOP
      iowrite32(0x8400, td3_i2c2_base + I2C_CON);

      // // disable interrupts
      // iowrite32(0xFF, td3_i2c2_base + I2C_IRQENABLE_CLR);


   // initialize IMU MPU6050
   if(init_control == 0)
   {
      init_control = 1;
      initMPU6050();
   }


   printk(KERN_INFO "|OPEN| [LOG] td3_i2c : Open OK\n");


   return 0;
}




/* ------------------------------------------------------------------------------
 * --- close
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_close
 *
 * @brief      File operation close of the I2C2 module driver. This function 
 *             reset the I2C2 registers written in the open.
 *
 * @param      struct inode *inode
 * @param      struct file *filp
 *
 * @return     static int
 *
 * ------------------------------------------------------------------------------
 */

static int td3_i2c_close(struct inode *inode, struct file *filp)
{
   printk(KERN_INFO "|CLOSE| [LOG] td3_i2c : Closing module...\n");


   if(atomic_read(&td3_i2c_ctrlopen_count) == 0)
   {
      // ---> write registers
         // reset registers
         // see the documentation on the header ---> ../inc/am335x.h
         iowrite32(0x0000, td3_i2c2_base + I2C_CON);
         iowrite32(0x00, td3_i2c2_base + I2C_PSC);
         iowrite32(0x00, td3_i2c2_base + I2C_SCLL);
         iowrite32(0x00, td3_i2c2_base + I2C_SCLH);
         iowrite32(0x00, td3_i2c2_base + I2C_OA);
         // iowrite32(0x00, td3_i2c2_base + I2C_SYSC);
   }


   // --> control device open
      printk(KERN_INFO "|CLOSE| [LOG] td3_i2c : Closing...\n");

   // wake up next process only if the device is close for all process
   // remember that the driver will be closed when the server process be kill
   if(atomic_dec_and_test(&td3_i2c_ctrlopen_count) == 1)
      atomic_set(&td3_i2c_wk_cond_ctrlopen, 1);

   wake_up_interruptible(&td3_i2c_wk_ctrlopen);

   printk(KERN_INFO "|CLOSE| [LOG] td3_i2c : Close OK\n");


   return 0;
}








/* ------------------------------------------------------------------------------
 * --- read
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_read
 *
 * @brief      File operation read of the I2C2 module driver. This function 
 *             reads one axis of the MPU6050's accelerometer.
 *             To read a device register, first must be transmited the register
 *             address. In order to perform this reads, two private functions
 *             are used, to send and receive byts. 
 *
 * @param      struct file *filep
 * @param      char *buffer
 * @param      size_t len
 * @param      loff_t *offset
 *
 * @return     static ssize_t
 *
 * ------------------------------------------------------------------------------
 */

static ssize_t td3_i2c_read(struct file *filep, char *buffer, size_t len, loff_t *offset)
{
   uint32_t status = 0;
   uint8_t data_send[2] = {0};
   uint16_t data_receive[2] = {0};
   int16_t data = 0;

   printk(KERN_INFO "........................................\n");
   printk(KERN_INFO "|READ| [LOG] td3_i2c : Reading module...\n");


   switch(Aaxis)
   {
      case AXIS_X:
         data_send[0] = MPU6050_RA_ACCEL_XOUT_H;   //0x3B
         data_send[1] = MPU6050_RA_ACCEL_XOUT_L;   //0x3C
         break;
      case AXIS_Y:
         data_send[0] = MPU6050_RA_ACCEL_YOUT_H;   //0x3D
         data_send[1] = MPU6050_RA_ACCEL_YOUT_L;   //0x3E
         break;
      case AXIS_Z:
         data_send[0] = MPU6050_RA_ACCEL_ZOUT_H;   //0x3F
         data_send[1] = MPU6050_RA_ACCEL_ZOUT_L;   //0x40
         break;
      default:
         break;
   }


   // increase read counter
   read_count++;


   // read the MSB of the accelerometer register 
   td3_i2c_writebyte(data_send[0]);
   data_receive[0] = td3_i2c_readbyte();

   // read the LSB of the accelerometer register 
   td3_i2c_writebyte(data_send[1]);
   data_receive[1] = td3_i2c_readbyte();

   // 16 bits data
   data = (uint16_t)(data_receive[0] << 8 | data_receive[1]);


   status = copy_to_user(buffer, (const void *) &data, sizeof(data));

   if(status != 0)
   {
      printk(KERN_ERR "|READ| [ERROR] td3_i2c : write %d (%s %d)\n", status, __FUNCTION__, __LINE__);
      return -1;
   }


   printk(KERN_INFO "|READ| [LOG] td3_i2c : Read OK\n");
   printk(KERN_INFO "........................................\n");


   return 2;
}




/* ------------------------------------------------------------------------------
 * --- write
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_write
 *
 * @brief      File operation write of the I2C2 module driver. This function 
 *             receive the user's command.
 *             That command allows the user to choose one accelerometer axis to
 *             be readed.
 *
 * @param      struct file *filep
 * @param      char *buffer
 * @param      size_t len
 * @param      loff_t *offset
 *
 * @return     static ssize_t
 *
 * ------------------------------------------------------------------------------
 */

static ssize_t td3_i2c_write(struct file *filep, const char *buffer, size_t len, loff_t *offset)
{
   uint32_t status = 0;
   uint8_t data = 0;


   printk(KERN_INFO "|WRITE| [LOG] td3_i2c : Writing module...\n");
 

   // load the data to write
   status = copy_from_user((void *) &data, (const void *) buffer, 1);

   if(status != 0)
   {
      printk(KERN_ERR "|WRITE| [ERROR] td3_i2c : write %d (%s %d)\n", status, __FUNCTION__, __LINE__);
      return -1;
   }

   // printk(KERN_DEBUG "|WRITE| [DEBUG] Data to write = 0x%x\n", data);


   // load the user's option
   if((data >= AXIS_X) && (data <= AXIS_Z))
   {
      Aaxis = data;

      printk(KERN_INFO "|WRITE| [LOG] td3_i2c : Write OK\n");
      
      // one byte readed
      return 1;
   }
   else
   {
      printk(KERN_ERR "|WRITE| [ERROR] td3_i2c : WRONG COMMAND\n");
      
      // zero byte readed, wrong command
      return 0;
   }
}








/* ------------------------------------------------------------------------------
 * --- ioctl
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_ioctl
 *
 * @brief      File operation ioctl of the I2C2 module driver. This function 
 *             returns the quantity of reads.
 *
 * @param      struct file *filep
 * @param      unsigned int cmd
 * @param      unsigned long arg
 *
 * @return     static int
 *
 * ------------------------------------------------------------------------------
 */

static long td3_i2c_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
   uint32_t status = 0;


   printk(KERN_INFO "|IOCTL| [LOG] td3_i2c : Running IOCTL...\n");
 

   // check cmd
   if(cmd == IOCTL_CANT)
   {
      status = copy_to_user((void *) arg, (const void *) &read_count, sizeof(read_count));

      if(status != 0)
      {
         printk(KERN_ERR "|IOCTL| [ERROR] td3_i2c : ioctl %d (%s %d)\n", status, __FUNCTION__, __LINE__);
         return -1;
      }


      printk(KERN_INFO "|IOCTL| [LOG] td3_i2c : IOCTL OK\n");
      
      // successful
      return 0;
   }
   else
   {
      printk(KERN_ERR "|IOCTL| [ERROR] td3_i2c : WRONG COMMAND\n");
      
      // zero byte readed, wrong command
      return -1;
   }
}



