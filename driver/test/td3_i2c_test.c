/** 
   \file    td3_i2c_test.c   
   \version 01.00
   \brief   Drive test program
            - caracteristica 1
            - caracteristica 2

   \author  Agustin Diaz Antuna
   \date    31/01/2019
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>      // open()
#include <unistd.h>     // close()
#include <stdint.h>     // uint16_t
#include <sys/ioctl.h>


/* ------------------------------------------------------------------------------
 * --- define
 * ------------------------------------------------------------------------------
 */

#define DRIVER_PATH                 "/dev/td3_i2c"
#define MPU6050_WHO_I_AM            0x75
#define MPU6050_RA_ACCEL_XOUT_H     0x3B
#define MPU6050_RA_ACCEL_XOUT_L     0x3C
#define MPU6050_RA_ACCEL_YOUT_H     0x3D
#define MPU6050_RA_ACCEL_YOUT_L     0x3E
#define MPU6050_RA_ACCEL_ZOUT_H     0x3F
#define MPU6050_RA_ACCEL_ZOUT_L     0x40
#define ARES_2G                     2.0f/32768.0f
#define IOCTL_CANT   1


/* ------------------------------------------------------------------------------
 * --- main
 * ------------------------------------------------------------------------------
 */

int main(void)
{
   int fd = 0;
   char data_send = 0;
   uint32_t op = 0;
   float data_receive = 0;
   int16_t aux = 0;


      // ****************************************************
      // opening driver
      printf("Opening driver: %s\n--->\t", DRIVER_PATH);

      if ((fd = open(DRIVER_PATH, O_RDWR))< 0)
      {
         printf("Cannot open driver\n");

         return 1;
      }

      printf("Successfully opened\n\n");



   while(1)
   {
      printf("****************************************************\n");
      printf("1: ACCEL_X \n2: ACCEL_Y\n3: ACCEL_Z\nOption: ");
//      op = getchar();
      scanf("%d", &op);

      data_send = op;




      // ****************************************************
      // writing driver
      printf("\nWriting driver\n--->\t");

      printf ("Data write: %d\n\n", data_send);

      write(fd, (char *) &data_send, sizeof(char));

      printf ("Data write: %d\n\n", data_send);




      // ****************************************************
      // reading driver
      printf("Reading driver\n--->\t");

      read(fd, &aux, sizeof(aux));

      data_receive = (float)(aux) * ARES_2G;

      printf ("Data read: %f\n\n", data_receive);




      // ****************************************************
      // ioctling driver
      printf("IOCTLing driver\n--->\t");

      ioctl(fd, IOCTL_CANT, &aux);

      printf ("Read counter: %d\n\n", aux);



      // sleep(2);
   }

      // ****************************************************
      // closing driver
      printf("Closing driver\n--->\t");

      close(fd);

      printf ("Driver closed\n\n");




   return 0;
}