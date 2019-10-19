/** 
   \file    client_threads.c   
   \version 01.00
   \brief   This file contains the threads created in the main client process
            - client_request
            - client_i_am_alive

   \author  Agustin Diaz Antuna
   \date    27/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
#include "client.h"


/* ------------------------------------------------------------------------------
 * --- client_request
 * ------------------------------------------------------------------------------
 * @fn         client_request
 *
 * @brief      Each of this processes is connected to one server. This thread:
 *             - sends the data frequency to the server
 *             - creates the i-am-alive thread
 *             - reads the socket and print the data on the screen
 *
 *
 * @param      void * arg     pointer to a serverctrl_t struct
 *
 * @return     void *
 *
 * ------------------------------------------------------------------------------
 */

void * client_request(void * arg)
{
   // servers control struct pointer
   serverctrl_t * server = ((serverctrl_t *) arg);

   // thread struct
   pthread_t thread;    pthread_attr_t attr = {0};
   int retvalue = 0;

   // set thread detatch
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);   

   // quantity of byte from read() or write()
   int bytescount = 0;

   // data to server
   datatoserver_t datatoserver = {0};
   datatoserver.frequency = server->frequency;

   // data from server
   datafromserver_t datafromserver = {0};
   struct tm *tm;


   // send data frequency
   if((bytescount = write(server->fd, &datatoserver.frequency,
                        sizeof(datatoserver.frequency))) == -1)
   {
      printf("---> Writing socket error <---\n");
      server->state = NOTCONNECTED;
      close(server->fd);
      printf("[%d] Server %s:%s connection close\n", server->number, server->ip, server->port);
      return 0;
   }


   // creates i_am_alive thread
   if((retvalue = pthread_create(&thread, &attr, client_i_am_alive,
                                 server)) != 0)
   {
      fprintf(stderr,"---> [ERROR] Thread creation error. ERROR CODE: %d\n", retvalue);
      exit(1);
   }
        



   while(1)
   {
      // read data from server
      if((bytescount = read(server->fd, &datafromserver, sizeof(datafromserver))) < 1)
      {
         printf("---> Reading socket error <---\n");

         // writing the original structure
         server->state = NOTCONNECTED;

         close(server->fd);
         printf("[%d] Server %s:%s connection close\n\n", server->number, server->ip, server->port);

         // must destroy thread
         return (void *) 1;
      }

      tm = localtime(&datafromserver.timer);

      printf("------------------------------------------\n");
      printf("-- [%d] Servidor %s:%s\t\t--\n", server->number, server->ip, server->port);
      printf("------------------------------------------\n");
      printf("-- Date: %d-%d-%d -- Time: %d:%d:%d\t--\n", tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
      printf("------------------------------------------\n");
      printf("-- Accelerometer AXIS X = %f\t--\n", datafromserver.accel[0]);
      printf("-- Accelerometer AXIS Y = %f\t--\n", datafromserver.accel[1]);
      printf("-- Accelerometer AXIS Z = %f\t--\n", datafromserver.accel[2]);
      printf("------------------------------------------\n\n");
   }


   return (void *) 1;
}




/* ------------------------------------------------------------------------------
 * --- client_its_alive
 * ------------------------------------------------------------------------------
 * @fn         client_i_am_alive
 *
 * @brief      This thread sends a i-am-alive message to the server every two
 *             seconds (server timeout)
 *
 *
 * @param      void * arg     pointer to a serverctrl_t struct
 *
 * @return     void *
 *
 * ------------------------------------------------------------------------------
 */

void * client_i_am_alive(void * arg)
{
   serverctrl_t * server = ((serverctrl_t *) arg);

   // quantity of byte from read() or write()
   int bytescount = 0;

   // data to server
   datatoserver_t datatoserver = {0};
   datatoserver.status = I_AM_ALIVE;


   while(1)
   {
      // sleep 1/5 of client timeout
      sleep(2);


      if(server->state == NOTCONNECTED)
      {
         return (void *) 0;
      }


      // send I_AM_ALIVE
      if((bytescount = write(server->fd, &datatoserver.status, sizeof(datatoserver.status))) < 1)
      {
         perror("---> Write socket error <---\n");

         // writing the original structure
         server->state = NOTCONNECTED;

         close(server->fd);
         printf("[%d] Server %s:%s connection close\n\n", server->number, server->ip, server->port);

         // must destroy thread
         return (void *) 0;
      }
   }


   return (void *) 1;
}



