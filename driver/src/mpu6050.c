/** 
   \file    mpu6050.c   
   \version 01.00
   \brief   This source file contains functions of the IMU MPU6050
            - init   -> initMPU6050

   \author  Agustin Diaz Antuna
   \date    29/01/2019
*/

/* ------------------------------------------------------------------------------
 * --- prototipos
 * ------------------------------------------------------------------------------
 */



/* ------------------------------------------------------------------------------
 * --- init
 * ------------------------------------------------------------------------------
 * @fn         initMPU6050
 *
 * @brief      Initialization function of MPU6050, wake up routine.
 *             Arduino's typical init function.
 *
 * @param      void
 *
 * @return     void
 *
 * ------------------------------------------------------------------------------
 */

void initMPU6050(void)
{  
   uint8_t c = 0;

   printk(KERN_INFO "[LOG] td3_i2c : Initializing MPU6050...\n");

   // Initialize MPU6050 device

   //  wake up device-don't need this here if using calibration function below
   td3_i2c_writebyte(MPU6050_RA_PWR_MGMT_1); // Clear sleep mode bit (6), enable all sensors 
   td3_i2c_writebyte(0x00);
   msleep(100); // Delay 100 ms for PLL to get established on x-axis gyro; should check for PLL ready interrupt  

   // get stable time source
   td3_i2c_writebyte(MPU6050_RA_PWR_MGMT_1);  // Set clock source to be PLL with x-axis gyroscope reference, bits 2:0 = 001
   td3_i2c_writebyte(0x01);


   // Configure Gyro and Accelerometer
   // Disable FSYNC and set accelerometer and gyro bandwidth to 44 and 42 Hz, respectively; 
   // DLPF_CFG = bits 2:0 = 010; this sets the sample rate at 1 kHz for both
   td3_i2c_writebyte(MPU6050_RA_CONFIG);  
   td3_i2c_writebyte(0x03);
 
   // Set sample rate = gyroscope output rate/(1 + SMPLRT_DIV)
   td3_i2c_writebyte(MPU6050_RA_SMPLRT_DIV);  // Use a 200 Hz sample rate 
   td3_i2c_writebyte(0x04);

   // Set gyroscope full scale range
   // Range selects FS_SEL and AFS_SEL are 0 - 3, so 2-bit values are left-shifted into positions 4:3
   td3_i2c_writebyte(MPU6050_RA_GYRO_CONFIG);
   c =  td3_i2c_readbyte();
   td3_i2c_writebyte(MPU6050_RA_GYRO_CONFIG); // Clear self-test bits [7:5] 
   td3_i2c_writebyte(c & ~0xE0);
   td3_i2c_writebyte(MPU6050_RA_GYRO_CONFIG); // Clear AFS bits [4:3]
   td3_i2c_writebyte(c & ~0x18);
   td3_i2c_writebyte(MPU6050_RA_GYRO_CONFIG); // Set full scale range for the gyro
   td3_i2c_writebyte(c | Gscale << 3);

   // Set accelerometer configuration
   td3_i2c_writebyte(MPU6050_RA_ACCEL_CONFIG);
   c =  td3_i2c_readbyte();
   td3_i2c_writebyte(MPU6050_RA_ACCEL_CONFIG); // Clear self-test bits [7:5] 
   td3_i2c_writebyte(c & ~0xE0);
   td3_i2c_writebyte(MPU6050_RA_ACCEL_CONFIG); // Clear AFS bits [4:3]
   td3_i2c_writebyte(c & ~0x18);
   td3_i2c_writebyte(MPU6050_RA_ACCEL_CONFIG); // Set full scale range for the accelerometer 
   td3_i2c_writebyte(c | Ascale << 3);

   // Configure Interrupts and Bypass Enable
   // Set interrupt pin active high, push-pull, and clear on read of INT_STATUS, enable I2C_BYPASS_EN so additional chips 
   // can join the I2C bus and all can be controlled by the Arduino as master
   td3_i2c_writebyte(MPU6050_RA_INT_PIN_CFG);    
   td3_i2c_writebyte(0x02);
   td3_i2c_writebyte(MPU6050_RA_INT_ENABLE);  // Enable data ready (bit 0) interrupt
   td3_i2c_writebyte(0x01);

   printk(KERN_INFO "[LOG] td3_i2c : MPU6050 initialization OK\n");
}



