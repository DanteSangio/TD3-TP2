/** 
   \file    td3_i2c_pdev.c   
   \version 01.00
   \brief   Platform device functions of the td3_i2c module driver.
            This file contains the following functions:
            - probe        -> td3_i2c_probe
            - remove       -> td3_i2c_remove
            - irq handler  -> td3_i2c_handler

   \author  Agustin Diaz Antuna
   \date    30/01/2019
*/


/* ------------------------------------------------------------------------------
 * --- probe
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_probe
 *
 * @brief      Function executed at the registration of the platform driver. This
 *             function must follow this steps:
 *             - Get the logical address of the I2C2 base register
 *             - Get the logical address of the clock register
 *             - Enable module clock
 *             - Get the logical address of the control module register
 *             - Configure the SDA and SCL pins
 *             - Get the IRQ from the platform driver
 *             - Request IRQ
 *
 * @param      struct platform_device *         contains platform driver info
 *
 * @return     status                           error control
 *
 * ------------------------------------------------------------------------------
 */

static int td3_i2c_probe(struct platform_device *pdev)
{
   uint32_t reg_value;

   printk(KERN_INFO "|PROBE| [LOG] td3_i2c : Running probe...\n");


   // logic address of the i2c2 registers from the device tree
   td3_i2c2_base = of_iomap(pdev->dev.of_node, 0);

   // printk(KERN_DEBUG "|PROBE| [DEBUG] td3_i2c2_base = 0x%x\n", (uint32_t) td3_i2c2_base);




   // ---> enable clock: CM_PER_I2C2_CLKCTRL
      // mapping memory clock register from physical address to logical address
      if((td3_cm_per_base = ioremap(CM_PER, CM_PER_len)) == NULL)
      {
         printk(KERN_ERR "|PROBE| [ERROR] td3_i2c : (%s %d)\n", __FUNCTION__, __LINE__);
         iounmap(td3_i2c2_base);
         return 1;
      }
      
      // modify register
      // enable module clock
      reg_value = ioread32(td3_cm_per_base + CM_PER_I2C2_CLKCTRL_off);
      reg_value |= 0x02;
      iowrite32(reg_value, td3_cm_per_base + CM_PER_I2C2_CLKCTRL_off);
      
      // wait until the clock is enable
      msleep(10);
      
      // check clock control register
      reg_value = ioread32(td3_cm_per_base + CM_PER_I2C2_CLKCTRL_off);
      if((reg_value & 0x00000003) != 0x02)
      { 
         printk(KERN_ERR "|PROBE| [ERROR] td3_i2c : (%s %d)\n", __FUNCTION__, __LINE__);
         iounmap(td3_i2c2_base);
         iounmap(td3_cm_per_base);
         return 1;
      }
      



   // ---> configure pinmux
      // mapping control module register from physical address to logical address
      if((td3_control_module_base = ioremap(CONTROL_MODULE, CONTROL_MODULE_len)) == NULL)
      {
         printk(KERN_ERR "|PROBE| [ERROR] td3_i2c : (%s %d)\n", __FUNCTION__, __LINE__);
         iounmap(td3_i2c2_base);
         iounmap(td3_cm_per_base);
         return 1;
      }

      // pin SCL
      // write register : fast - receiver enabled - pullup - pull disable - i2c2_scl
      iowrite32(0x3B, td3_control_module_base + conf_uart1_rtsn_off);
      
      // pin SDA
      // write register : fast - receiver enabled - pullup - pull disable - i2c2_scl
      iowrite32(0x3B, td3_control_module_base + conf_uart1_ctsn_off);




   // ---> get irq
      if((virq = platform_get_irq(pdev, 0)) < 0)
      {
         printk(KERN_ERR "|PROBE| [ERROR] td3_i2c : (%s %d)\n", __FUNCTION__, __LINE__);
         iounmap(td3_i2c2_base);
         iounmap(td3_cm_per_base);
         iounmap(td3_control_module_base);
         return 1;         
      }

      // printk(KERN_DEBUG "|PROBE| [DEBUG] virq = %d\n", virq);

      if(request_irq(virq, (irq_handler_t) td3_i2c_handler, IRQF_TRIGGER_RISING, NAME, NULL))
      {
         printk(KERN_ERR "|PROBE| [ERROR] td3_i2c : (%s %d)\n", __FUNCTION__, __LINE__);
         iounmap(td3_i2c2_base);
         iounmap(td3_cm_per_base);
         iounmap(td3_control_module_base);
         return 1;
      }




   printk(KERN_INFO "|PROBE| [LOG] td3_i2c : Probe OK\n");


   return 0;
}




/* ------------------------------------------------------------------------------
 * --- remove
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_remove
 *
 * @brief      Function executed at the unregistration of the platform driver. 
 *             This function must undo the steps performed in the probe:
 *             - Release IRQ
 *             - Unmap the logical address of the I2C2 base register
 *             - Unmap the logical address of the clock register
 *             - Unmap the logical address of the control module register
 *
 * @param      struct platform_device *         contains platform driver info
 *
 * @return     status                           error control
 *
 * ------------------------------------------------------------------------------
 */

static int td3_i2c_remove(struct platform_device *pdev)
{

   printk(KERN_INFO "|REMOVE| [LOG] td3_i2c : Removing module...\n");

   
   // free irq handler
   free_irq(virq, NULL);

   // unmap i2c2 base
   iounmap(td3_i2c2_base);

   // unmap CM_PER base
   iounmap(td3_cm_per_base);

   // unmap control_module base
   iounmap(td3_control_module_base);


   printk(KERN_INFO "|REMOVE| [LOG] td3_i2c : Remove module OK\n");


   return 0;
}




/* ------------------------------------------------------------------------------
 * --- handler
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_handler
 *
 * @brief      IRQ handler of the I2C2 module driver. 
 *             - Check interrupt source
 *             - Read/Write data
 *             - Clear interrupt flags
 *             - Disable rx/tx interrupt
 *             - Write wake-up-condition
 *
 * @param      int irq
 * @param      void *dev_id
 * @param      struct pt_regs *regs
 *
 * @return     irqreturn_t
 *
 * ------------------------------------------------------------------------------
 */

irqreturn_t td3_i2c_handler(int irq, void *dev_id, struct pt_regs *regs)
{
   uint32_t irq_status;
   uint32_t reg_value;

   printk(KERN_INFO "|IRQ| [LOG] td3_i2c : New interrupt...\n");


   // check irq source
   irq_status = ioread32(td3_i2c2_base + I2C_IRQSTATUS);


   if(irq_status & I2C_IRQSTATUS_RRDY)
   {
      printk(KERN_INFO "|IRQ| [LOG] td3_i2c : source -> RX\n");

      // read data
      td3_i2c_data_rx = ioread8(td3_i2c2_base + I2C_DATA);

      // printk(KERN_DEBUG "|IRQ| [DEBUG] td3_i2c_data_rx = 0x%x\n", td3_i2c_data_rx);


      // clear flags
      // GC_IE - XRDY_IE   - RRDY_IE   - ARDY_IE   - NACK_IE
      // 5     - 4         - 3         - 2         - 1
      // 10 1110 b = 0x27
      reg_value = ioread32(td3_i2c2_base + I2C_IRQSTATUS);
      reg_value |= 0x27;
      iowrite32(reg_value, td3_i2c2_base + I2C_IRQSTATUS);


      // disable rx interrupt
      reg_value = ioread32(td3_i2c2_base + I2C_IRQENABLE_CLR);
      reg_value |= I2C_IRQSTATUS_RRDY;
      iowrite32 (reg_value, td3_i2c2_base + I2C_IRQENABLE_CLR);


      // wake up read operation
      td3_i2c_wk_cond = 1;
      wake_up_interruptible(&td3_i2c_wk);
   }
   

   if(irq_status & I2C_IRQSTATUS_XRDY)
   {
      printk(KERN_INFO "|IRQ| [LOG] td3_i2c : source -> TX\n");
   
      // write data
      iowrite8(td3_i2c_data_tx, td3_i2c2_base + I2C_DATA);

      // printk(KERN_DEBUG "|IRQ| [DEBUG] td3_i2c_data_tx = 0x%x\n", td3_i2c_data_tx);


      // clear flags
      // GC_IE - XRDY_IE   - RRDY_IE   - ARDY_IE   - NACK_IE
      // 5     - 4         - 3         - 2         - 1
      // 11 0110 b = 0x36
      reg_value = ioread32(td3_i2c2_base + I2C_IRQSTATUS);
      reg_value |= 0x36;
      iowrite32(reg_value, td3_i2c2_base + I2C_IRQSTATUS);


      // disable tx interrupt
      reg_value = ioread32(td3_i2c2_base + I2C_IRQENABLE_CLR);
      reg_value |= I2C_IRQSTATUS_XRDY;
      // reg_value |= 0xFFFF;
      iowrite32 (reg_value, td3_i2c2_base + I2C_IRQENABLE_CLR);


      // wake up write operation
      td3_i2c_wk_cond = 1;
      wake_up_interruptible(&td3_i2c_wk);
   }


   // printk(KERN_DEBUG "|IRQ| [DEBUG] Clearing flags\n");

   // clear all flags
   irq_status = ioread32(td3_i2c2_base + I2C_IRQSTATUS);
   irq_status |= 0x3E;
   iowrite32(irq_status, td3_i2c2_base + I2C_IRQSTATUS);


   printk(KERN_INFO "|IRQ| [LOG] td3_i2c : IRQ handler OK\n");


   return IRQ_HANDLED;
}



