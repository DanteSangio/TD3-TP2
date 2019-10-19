/*
Driver: los registros para inicializar el SPI son los siguientes:

Primero configurar clock y pines.

Clock: 

CLK (Interface clock) 100 MHz CORE_CLKOUTM4 / 2 pd_per_l4ls_gclk. From PRCM
CLKSPIREF (Functional clock) 48 MHz PER_CLKOUTM2 / 4 pd_per_spi_gclk. From PRCM

Pines: 

SPIx_SCLK I/O SPI serial clock (output when master, input when slave)
SPIx_D0 I/O Can be configured as either input or output (MOSI or MISO)
SPIx_D1 I/O Can be configured as either input or output (MOSI or MISO)
SPIx_CS0 I/O SPI chip select 0 output when master, input when slave (active low)
SPIx_CS1 O SPI chip select 1 output when master, input when slave (active low)

SPI:

   Registros importantes: 

      MCSPI_SYSCONFIG
      MCSPI_MODULCTRL : Control Module 0x44E1_0000 -> 0x44E1_1FFF 128KB 
      MCSPI_CH0CONF
      MCSPI_CH0CTRL

En la hoja de datos estan mas o menos por la pagina 4900

MCSPI_REVISION McSPI revision register 
*   MCSPI_SYSCONFIG McSPI system configuration register 
MCSPI_SYSSTATUS McSPI system status register 
MCSPI_IRQSTATUS McSPI interrupt status register 
MCSPI_IRQENABLE McSPI interrupt enable register 
MCSPI_SYST McSPI system register 
*   MCSPI_MODULCTRL McSPI module control register 
*   MCSPI_CH0CONF McSPI channel 0 configuration register 
MCSPI_CH0STAT McSPI channel 0 status register 
*   MCSPI_CH0CTRL McSPI channel 0 control register 
MCSPI_TX0 McSPI channel 0 FIFO transmit buffer register 
MCSPI_RX0 McSPI channel 0 FIFO receive buffer register
MCSPI_CH1CONF McSPI channel 1 configuration register 
MCSPI_CH1STAT McSPI channel 1 status register 
MCSPI_CH1CTRL McSPI channel 1 control register 
MCSPI_TX1 McSPI channel 1 FIFO transmit buffer register 
MCSPI_RX1 McSPI channel 1 FIFO receive buffer register 
MCSPI_CH2CONF McSPI channel 2 configuration register 
MCSPI_CH2STAT McSPI channel 2 status register 
MCSPI_CH2CTRL McSPI channel 2 control register 
MCSPI_TX2 McSPI channel 2 FIFO transmit buffer register 
MCSPI_RX2 McSPI channel 2 FIFO receive buffer register 
MCSPI_CH3CONF McSPI channel 3 configuration register 
MCSPI_CH3STAT McSPI channel 3 status register 
MCSPI_CH3CTRL McSPI channel 3 control register 
MCSPI_TX3 McSPI channel 3 FIFO transmit buffer register 
MCSPI_RX3 McSPI channel 3 FIFO receive buffer register 
MCSPI_XFERLEVEL McSPI transfer levels register 
MCSPI_DAFRX McSPI DMA address aligned FIFO receiver register

Ejemplo de inicializacion de SPI canal 0: 

   Registros Relevantes:

      MCSPI_SYSSTATUS -> 

      MCSPI_SYSCONFIG -> 

Ejemplo de inicializacion de SPI canal 1: 

   Relevant Registers:

      CM_PER_SPI1_CLKCTRL @ 0x44E00050 
         Enable the Clock

      MCSPI_SYSCONFIG @ 0x481A0110
         Reset the SPI

      MCSPI_SYSSTATUS @ 0x481a0114
         Check Reset Completed

      MCSPI_MODULCTRL @ 0x481A0128
         Turn DMA off (for now) 

      MCSPI_SYSCONFIG @ 0x481A0110
         Automatic OCP clock gating strategy is applied, based on theOCP interface activity. 
         Smart-idle mode: local target's idle state eventually follows(acknowledges) the system's idle requests, depending on the IPmodule's internal requirements.
         Clocks activity during wake-up mode period.3h = OCP and Functional clocks are maintained.

      MCSPI_IRQSTATUS @ 0x481A0118
         Disable Interrupts

      MCSPI_CH1CTRL  @ 0x481A0148
         Turn on/off SPI CH1

      MCSPI_CH1CTRL  @ 0x481A0140
         Set polarity, phases, frequencies, word length, tx/rx registers, etc

      MCSPI_CH1STAT @ 0x481A0144
         Check buffer empty/full

      MCSPI_TX1 @ 0x481A014C
         Load in data to transfer

      MCSPI_XFERLEVEL @  0x481A017C
         Could set this up to take advantage of larger FIFO sizes.  Currently not being used


Mapa de memoria: 

McSPI0 Registers : 0x4803_0000 -> 0x4803_0FFF (4KB)
McSPI1 Registers : 0x481A_0000 -> 0x481A_0FFF (4KB)



CM_PER 0x44E0_0000 0x44E0_03FF 1KB Clock Module Peripheral Registers

   CM_PER_SPI0_CLKCTRL : offset 4Ch
   CM_PER_SPI1_CLKCTRL : offset 50h

   CM_PER_LENTH : 0x0400

CM_WKUP 0x44E0_0400 0x44E0_04FF 256 Bytes Clock Module Wakeup Registers
CM_DPLL 0x44E0_0500 0x44E0_05FF 256 Bytes Clock Module PLL Registers
CM_MPU 0x44E0_0600 0x44E0_06FF 256 Bytes Clock Module MPU Registers
CM_DEVICE 0x44E0_0700 0x44E0_07FF 256 Bytes Clock Module Device Registers
CM_RTC 0x44E0_0800 0x44E0_08FF 256 Bytes Clock Module RTC Registers
CM_GFX 0x44E0_0900 0x44E0_09FF 256 Bytes Clock Module Graphics Controller Registers
CM_CEFUSE 0x44E0_0A00 0x44E0_0AFF 256 Bytes Clock Module Efuse Registers
PRM_IRQ 0x44E0_0B00 0x44E0_0BFF 256 Bytes Power Reset Module Interrupt Registers
PRM_PER 0x44E0_0C00 0x44E0_0CFF 256 Bytes Power Reset Module Peripheral Registers
PRM_WKUP 0x44E0_0D00 0x44E0_0DFF 256 Bytes Power Reset Module Wakeup Registers
PRM_MPU 0x44E0_0E00 0x44E0_0EFF 256 Bytes Power Reset Module MPU Registers 
PRM_DEV 0x44E0_0F00 0x44E0_0FFF 256 Bytes Power Reset Module Device Registers
PRM_RTC 0x44E0_1000 0x44E0_10FF 256 Bytes Power Reset Module RTC Registers
PRM_GFX 0x44E0_1100 0x44E0_11FF 256 Bytes Power Reset Module Graphics Controller Registers
PRM_CEFUSE 0x44E0_1200 0x44E0_12FF 256 Bytes Power Reset Module Efuse Registers

*/




/** 
   \file    am335x.h   
   \version 01.00
   \brief   Header file of the microcontroller AM335x from the BBB.
            - i2c2 register address
            - some macros

   \author  Agustin Diaz Antuna
   \date    30/01/2019
*/

/* ------------------------------------------------------------------------------
 * --- define
 * ------------------------------------------------------------------------------
 */

// memory map from page 177

// i2c2 clock
   /*
      CM_PER_I2C2_CLKCTRL[1:0]   --- pp 1178

      0x0 = DISABLED : Module is disable by SW. Any OCP access to module results in
      an error, except if resulting from a module wakeup (asynchronous wakeup).
      0x1 = RESERVED_1 : Reserved
      0x2 = ENABLE : Module is explicitly enabled. Interface clock (if not used for 
      functions) may be gated according to the clock domain state. Functional clocks 
      are guarantied to stay present. As long as in this configuration, power domain 
      sleep transition cannot happen.
      0x3 = RESERVED : Reserved
   */

   #define CM_PER                   0x44E00000
   #define CM_PER_len               0x0400

   #define CM_PER_I2C2_CLKCTRL_off  0x0044




// pinmux: i2c2 peripheral use pinout of uart1  --- pp 1368
   /*
      conf_uart1_rtsn   --- pp 1422
      conf_uart1_ctsn   --- pp 1422

      fast - receiver enabled - pullup - pull_disable - i2c2_scl
      011 1011 b = 0x3B

      +-------+-------------------------------+------+-------+-------------------------------------------+
      |  BIT  |             FIELD             | TYPE | RESET |                DESCRIPTION                |
      +-------+-------------------------------+------+-------+-------------------------------------------+
      | 31-20 | Reserved                      | R    | 0h    |                                           |
      | 19-7  | Reserved                      | R    | 0h    |                                           |
      | 6     | conf_<module>_<pin>_slewctrl  | R/W  | X     | Select between faster or slower slew rate |
      |       |                               |      |       | 0: Fast                                   |
      |       |                               |      |       | 1: Slow                                   |
      |       |                               |      |       | Reset value is pad-dependent.             |
      | 5     | conf_<module>_<pin>_rx active | R/W  | 1h    | Input enable value for the PAD            |
      |       |                               |      |       | 0: Receiver disabled                      |
      |       |                               |      |       | 1: Receiver enabled                       |
      | 4     | conf_<module>_<pin>_putypesel | R/W  | X     | Pad pullup/pulldown type selection        |
      |       |                               |      |       | 0: Pulldown selected                      |
      |       |                               |      |       | 1: Pullup selected                        |
      |       |                               |      |       | Reset value is pad-dependent.             |
      | 3     | conf_<module>_<pin>_puden     | R/W  | X     | Pad pullup/pulldown enable                |
      |       |                               |      |       | 0: Pullup/pulldown enabled                |
      |       |                               |      |       | 1: Pullup/pulldown disabled               |
      |       |                               |      |       | Reset value is pad-dependent.             |
      | 2-0   | conf_<module>_<pin>_mmode     | R/W  | X     | Pad functional signal mux select          |
      |       |                               |      |       | Reset value is pad-dependent.             |
      +-------+-------------------------------+------+-------+-------------------------------------------+
   */
   // SCL -> uart1_rtsn
   // SDA -> uart1_ctsn

   #define CONTROL_MODULE           0x44E10000
   #define CONTROL_MODULE_len       0x2000
   
   #define conf_uart1_rtsn_off      0x097C
   #define conf_uart1_ctsn_off      0x0978




// i2c register   --- pp 4490
   /* I2C_CON = 0x00       RESET MODE
      I2C_CON = 0x8400     ENABLE & MASTER & RX & STOP
      I2C_CON = 0x8600     ENABLE & MASTER & TX & STOP

      [15]     I2C_EN      0h = Controller in reset. FIFO are cleared and
                           status bits are set to their default value.
                           1h = Module enabled
      [13-12]  OPMODE      0h = I2C Fast/Standard mode
      [11]     STB         0h = Normal mode
      [10]     MST         0h = Slave mode
                           1h = Master mode
      [9]      TRX         0h = Receiver mode
      [8]      XSA         0h = 7-bit address mode
      [7]      XOA0        0h = 7-bit address mode
      [6]      XOA1        0h = 7-bit address mode
      [5]      XOA2        0h = 7-bit address mode. (I2C mode only).
      [4]      XOA3        0h = 7-bit address mode
      [1]      STP         0h = No action or stop condition detected
      [0]      STT         0h = No action or start condition detected
   */

   /* I2C_PSC = 0x01

      [7-0]    PSC         The core logic is sampled at the clock rate of 
                           the system clock for the module divided by 
                           (PSC + 1).
      
      48MHz / (PSC+3) = 48MHz / (1+1) = 24MHz
   */

   /* I2C_SCLL = 0x35

      [7-0]    SCLL        This 8-bit value is used to generate the SCL 
                           low time value (tLOW) when the peripheral is
                           operated in master mode.
                           tLOW = (SCLL + 7) * ICLK time period
                           Value after reset is low (all 8 bits).
      
      Expect a tLOW = 1/400kHz to 200kHz SCLK

      tLOW = (SCLL + 7) * ICLK
      SCLL = (tLOW / ICLK) - 7 = ( (1/400kHz) / (1/24MHz) ) - 7
           = (0,0000025 / 0,000000042) - 7 = 52,52 -> 0x34,8 -> 0x35
   */

   /* I2C_SCLH = 0x37

      [7-0]    SCLH        This 8-bit value is used to generate the SCL
                           high time value (tHIGH) when the peripheral is
                           operated in master mode.
                           tHIGH = (SCLH + 5) * ICLK time period.
                           Value after reset is low (all 8 bits).
      
      Expect a tHIGH = 1/400kHz to 200kHz SCLK

      tHIGH = (SCLH + 5) * ICLK
      SCLH = (tHIGH / ICLK) - 5 = ( (1/400kHz) / (1/24MHz) ) - 5
           = (0,0000025 / 0,000000042) - 5 = 54,52 -> 0x36,8 -> 0x37
   */

   /* I2C_OA = 0x77

      [7-0]    OA          Own address.
                           This field specifies either: 
                           10-bit address coded on OA
      
      Random address
   */

   /* I2C_SYSC = 0x08

      [9-8]    CLKACTIVITY 0h = Both clocks can be cut off
      [4-3]    IDLEMODE    1h = No Idle mode
      [2]      ENAWAKEUP   0h = Wakeup mechanism is disabled
      [1]      SRST        0h = Normal mode
      [0]      AUTOIDLE    0h = Auto Idle mechanism is disabled
   */

   #define I2C2_len              0x1000

   #define I2C_REVNB_LO          0x00
   #define I2C_REVNB_HI          0x04
   #define I2C_SYSC              0x10     // --- pp 4493
   #define I2C_IRQSTATUS_RAW     0x24     // --- pp 4495
   #define I2C_IRQSTATUS         0x28
   #define I2C_IRQENABLE_SET     0x2C
   #define I2C_IRQENABLE_CLR     0x30
   #define I2C_WE                0x34
   #define I2C_DMARXENABLE_SET   0x38
   #define I2C_DMATXENABLE_SET   0x3C
   #define I2C_DMARXENABLE_CLR   0x40
   #define I2C_DMATXENABLE_CLR   0x44
   #define I2C_DMARXWAKE_EN      0x48
   #define I2C_DMATXWAKE_EN      0x4C
   #define I2C_SYSS              0x90
   #define I2C_BUF               0x94
   #define I2C_CNT               0x98
   #define I2C_DATA              0x9C
   #define I2C_CON               0xA4     // --- pp 4523
   #define I2C_OA                0xA8     // --- pp 4526
   #define I2C_SA                0xAC
   #define I2C_PSC               0xB0     // --- pp 4528
   #define I2C_SCLL              0xB4     // --- pp 4529
   #define I2C_SCLH              0xB8     // --- pp 4530
   #define I2C_SYSTEST           0xBC
   #define I2C_BUFSTAT           0xC0
   #define I2C_OA1               0xC4
   #define I2C_OA2               0xC8
   #define I2C_OA3               0xCC
   #define I2C_ACTOA             0xD0
   #define I2C_SBLOCK            0xD4


   // bus busy
   #define BB                 1
   #define TRANSMIT_READY     1
   #define RECEPTION_READY    1

   // IRQSTATUS register
   #define I2C_IRQSTATUS_RRDY 0x08
   #define I2C_IRQSTATUS_XRDY 0x10

   // CON register
   #define I2C_CON_START         1
   #define I2C_CON_STOP          2



