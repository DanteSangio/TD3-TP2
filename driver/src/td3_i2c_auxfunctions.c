/** 
   \file    td3_i2c_auxfunctions.c   
   \version 01.00
   \brief   Auxiliar functions of td3_i2c device driver
            - debug        -> td3_i2c_register_debug
            - write byte   -> td3_i2c_writebyte
            - read byte    -> td3_i2c_readbyte

   \author  Agustin Diaz Antuna
   \date    29/01/2019
*/


/* ------------------------------------------------------------------------------
 * --- register debug
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_register_debug
 *
 * @brief      Debug functions. Print all I2C2 registers.
 *
 * @param      void
 *
 * @return     void
 *
 * ------------------------------------------------------------------------------
 */

void td3_i2c_register_debug(void)
{
   uint32_t reg_value = 0;


   printk(KERN_DEBUG "//////////////////////////////////////////////");

   reg_value = ioread32(td3_cm_per_base + CM_PER_I2C2_CLKCTRL_off);
   printk(KERN_DEBUG "===> [DEBUG] : CM_PER_I2C2_CLKCTRL_off = %x\n", reg_value);
   reg_value = ioread32(td3_control_module_base + conf_uart1_ctsn_off);
   printk(KERN_DEBUG "===> [DEBUG] : conf_uart1_ctsn_off = %x\n", reg_value);
   reg_value = ioread32(td3_control_module_base + conf_uart1_rtsn_off);
   printk(KERN_DEBUG "===> [DEBUG] : conf_uart1_rtsn_off = %x\n", reg_value);

   printk(KERN_DEBUG "//////////////////////////////////////////////");

   reg_value = ioread32(td3_i2c2_base + I2C_REVNB_LO);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_REVNB_LO = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_REVNB_HI);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_REVNB_HI = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_SYSC);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_SYSC = %x\n", reg_value);

   reg_value = ioread32(td3_i2c2_base + I2C_IRQSTATUS_RAW);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_IRQSTATUS_RAW = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_IRQSTATUS);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_IRQSTATUS = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_IRQENABLE_SET);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_IRQENABLE_SET = %x\n",reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_IRQENABLE_CLR);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_IRQENABLE_CLR = %x\n",reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_WE);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_WE = %x\n", reg_value);

   reg_value = ioread32(td3_i2c2_base + I2C_DMARXENABLE_SET);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_DMARXENABLE_SET = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_DMATXENABLE_SET);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_DMATXENABLE_SET = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_DMARXENABLE_CLR);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_DMARXENABLE_CLR = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_DMATXENABLE_CLR);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_DMATXENABLE_CLR = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_DMARXWAKE_EN);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_DMARXWAKE_EN = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_DMATXWAKE_EN);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_DMATXWAKE_EN = %x\n", reg_value);

   reg_value = ioread32(td3_i2c2_base + I2C_SYSS);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_SYSS = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_BUF);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_BUF = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_CNT);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_CNT = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_DATA);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_DATA = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_CON);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_CON = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_OA);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_OA = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_SA);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_SA = %x\n", reg_value);

   reg_value = ioread32(td3_i2c2_base + I2C_PSC);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_PSC = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_SCLL);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_SCLL = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_SCLH);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_SCLH = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_SYSTEST);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_SYSTEST = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_BUFSTAT);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_BUFSTAT = %x\n", reg_value);

   reg_value = ioread32(td3_i2c2_base + I2C_OA1);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_OA1 = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_OA2);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_OA2 = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_OA3);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_OA3 = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_ACTOA);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_ACTOA = %x\n", reg_value);
   reg_value = ioread32(td3_i2c2_base + I2C_SBLOCK);
   printk(KERN_DEBUG "===> [DEBUG] : I2C_SBLOCK = %x\n", reg_value);

   printk(KERN_DEBUG "//////////////////////////////////////////////");

}





/* ------------------------------------------------------------------------------
 * --- i2c write byte
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_writebyte
 *
 * @brief      Write byte. This private function, allow the file-operation-
 *             functions to send a byte to MPU6050.
 *
 * @param      uint8_t data      byte to write
 *
 * @return     void
 *
 * ------------------------------------------------------------------------------
 */

void td3_i2c_writebyte(uint8_t data)
{
   /*
      21.3.15.6 Transmit Data

      Poll the transmit data ready interrupt flag bit (XRDY) in the I2C status register
      (I2C_IRQSTATUS_RAW), use the XRDY interrupt (I2C_IRQENABLE_SET.XRDY_IE set) or use 
      the DMA TX (I2C_BUF.XDMA_EN set together with I2C_DMATXENABLE_SET) 
      to write data into 
      the data transmit register (I2C_DATA). Use draining feature (I2C_IRQSTATUS_RAW.XDR 
      enabled by I2C_IRQENABLE_SET.XDR_IE)) if the transfer length is not equal with FIFO 
      threshold.
   */

   uint32_t i = 0;
   uint32_t reg_value = 0;
   uint32_t status = 0;


   printk(KERN_INFO "|writebyte| [LOG] td3_i2c : Sending byte...\n");
 

   // ---> byte transmission
      // printk(KERN_DEBUG "|writebyte| [DEBUG] Checking if busy\n");
 
      // check if busy
      reg_value = ioread32(td3_i2c2_base + I2C_IRQSTATUS_RAW);

      while((reg_value >> 12) & 1)
      {
         msleep(100);

         printk(KERN_ERR "|writebyte| [ERROR] td3_i2c : Device busy\n");

         i++;

         if(i == 4)
         {
            printk(KERN_ERR "|writebyte| [ERROR] td3_i2c : busy (%s %d)\n", __FUNCTION__, __LINE__);         
            return;
         }
      }

      // printk(KERN_DEBUG "|writebyte| [DEBUG] Device ready\n");


      // load data
      td3_i2c_data_tx = data;

      // data len = 1 byte
      iowrite32(1, td3_i2c2_base + I2C_CNT);

      // configure register -> ENABLE & MASTER & TX & STOP
      reg_value = ioread32(td3_i2c2_base + I2C_CON);
      reg_value |= 0x600;
      iowrite32(reg_value, td3_i2c2_base + I2C_CON);

      // enable tx interrupt
      iowrite32(I2C_IRQSTATUS_XRDY, td3_i2c2_base + I2C_IRQENABLE_SET);

      // configuration register -> START [1:0] = 01 b
      reg_value = ioread32(td3_i2c2_base + I2C_CON);
      reg_value |= I2C_CON_START;
      iowrite32(reg_value, td3_i2c2_base + I2C_CON);

      // printk(KERN_DEBUG "|writebyte| [DEBUG] waiting event interruptible (TX)\n");

      // wait until transmission ends
      if((status = wait_event_interruptible(td3_i2c_wk, td3_i2c_wk_cond > 0)) < 0)
      {
         td3_i2c_wk_cond = 0;
         printk(KERN_ERR "|writebyte| [ERROR] td3_i2c : read (%s %d)\n", __FUNCTION__, __LINE__);
         return;
      }

      td3_i2c_wk_cond = 0;

      printk(KERN_INFO "|writebyte| [LOG] td3_i2c : TX OK\n");


      // configuration register -> STOP [1:0] = 10 b
      reg_value = ioread32(td3_i2c2_base + I2C_CON);
      reg_value &= 0xFFFFFFFE;
      reg_value |= I2C_CON_STOP;
      iowrite32(reg_value, td3_i2c2_base + I2C_CON);


      msleep(1);


      printk(KERN_INFO "|writebyte| [LOG] td3_i2c : Byte transmission OK\n");

}




/* ------------------------------------------------------------------------------
 * --- i2c read byte
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_readbyte
 *
 * @brief      Read byte. This private function, allow the file-operation-
 *             functions to read a byte from MPU6050.
 *
 * @param      void
 *
 * @return     uint8_t       byte readed
 *
 * ------------------------------------------------------------------------------
 */

uint8_t td3_i2c_readbyte(void)
{
   uint32_t i = 0;
   uint32_t reg_value = 0;
   uint32_t status = 0;
   uint8_t newdata;


   printk(KERN_INFO "|readbyte| [LOG] td3_i2c : Reading byte...\n");


   // check if busy
   // printk(KERN_DEBUG "|readbyte| [DEBUG] Checking if busy\n");

   reg_value = ioread32(td3_i2c2_base + I2C_IRQSTATUS_RAW);
   while((reg_value >> 12) & 1)
   {
      msleep(100);

      printk(KERN_ERR "|readbyte| [ERROR] td3_i2c : Device busy\n");

      i++;
      if(i >= 4)
      {
         printk(KERN_ERR "|readbyte| [ERROR] td3_i2c : busy (%s %d)\n", __FUNCTION__, __LINE__);
         return -1;
      }
   }

   // printk(KERN_DEBUG "|readbyte| [DEBUG] Module isn't busy\n");


   // data lenght = 1 byte
   iowrite32(1, td3_i2c2_base + I2C_CNT);

   // configure register -> ENABLE & MASTER & RX & STOP
   reg_value = ioread32(td3_i2c2_base + I2C_CON);
   reg_value = 0x8400;
   iowrite32(reg_value, td3_i2c2_base + I2C_CON);

   // enable rx interrupt
   iowrite32(I2C_IRQSTATUS_RRDY, td3_i2c2_base + I2C_IRQENABLE_SET);

   // configuration register -> START [1:0] = 01 b
   reg_value = ioread32(td3_i2c2_base + I2C_CON);
   reg_value &= 0xFFFFFFFC;
   reg_value |= I2C_CON_START;
   iowrite32(reg_value, td3_i2c2_base + I2C_CON);


   // printk(KERN_DEBUG "|readbyte| [DEBUG] Waiting event interruptible\n");

   // wait until reception ends
   if((status = wait_event_interruptible(td3_i2c_wk, td3_i2c_wk_cond > 0)) < 0)
   {
      td3_i2c_wk_cond = 0;
      printk(KERN_ERR "|readbyte| [ERROR] td3_i2c : read (%s %d)\n", __FUNCTION__, __LINE__);
      return status;
   }

   td3_i2c_wk_cond = 0;


   printk(KERN_INFO "|readbyte| [LOG] td3_i2c : RX OK\n");


   // printk(KERN_DEBUG "|readbyte| [DEBUG] data = 0x%x\n", td3_i2c_data_rx);


   // configure register -> STOP [1]
   reg_value = ioread32(td3_i2c2_base + I2C_CON);
   reg_value &= 0xFFFFFFFE;
   reg_value |= I2C_CON_STOP;
   iowrite32(reg_value, td3_i2c2_base + I2C_CON);

   newdata = td3_i2c_data_rx;

   printk(KERN_INFO "|readbyte| [LOG] td3_i2c : Byte reception OK\n");

   return newdata;
}



