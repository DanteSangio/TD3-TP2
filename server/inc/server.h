/** 
   \file    server.h   
   \version 01.00
   \brief   Header file of the TCP/IP server

   \author  Agustin Diaz Antuna
   \date    27/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
   #include "socklib_server.h"   // APIs from socklib_client
   #include <pthread.h>          // thread

   #include <fcntl.h>            // open()
   #include <sys/ioctl.h>        // ioctl()

   // time stamp
   #include <time.h>
   #include <sys/wait.h>      // signal()


/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
   #define DRIVER_PATH     "/dev/td3_spi"

   // ---> MPU6050
      #define ARES_2G            2.0f/32768.0f
      #define IOCTL_CANT         1
      
      #define MPU6050_ACCEL_X    1
      #define MPU6050_ACCEL_Y    2
      #define MPU6050_ACCEL_Z    3
   
   // ---> server
      #define BUFFERSIZE         3

      #define CONNECTED          1
      #define NOTCONNECTED       0


/* ------------------------------------------------------------------------------
 * --- prototipos
 * ------------------------------------------------------------------------------
 */

   // Funcion que ejecuta cada thread
   void * client_request_handler(void * arg);
   void * server_accept(void * arg);
   void * client_timeout(void * arg);
   void * td3_i2c_handler(void * arg);



