/** 
   \file    server.c   
   \version 01.00
   \brief   Main file of the TCP/IP server.
            - This program needs some arguments to work. These will be entered in
            the command line, as PORT and driver configuration
               - driver configuration:
                  1 = axis x
                  2 = axis y
                  3 = axis z
                  4 = all axis
            - Here the TCP/IP socket is created and binded to one thread
            - It creates a thread to wait for every new client
            - To close the server, you must enter 'x' + 'intro'

   \author  Agustin Diaz Antuna
   \date    09/10/2018
*/

/*

Vamos a leer una propiedad del device tree ubicado en:

   /proc/device-tree

Donde por ejemplo tenemos las propiedades de:

   model
   name 
   compatible

*/


/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
#include "server.h"


/* ------------------------------------------------------------------------------
 * --- main
 * ------------------------------------------------------------------------------
 * @fn         main
 *
 * @brief      Main process of the TCP/IP server
 *
 * @param      int argc
 * @param      char * argv[]
 *
 * @return     int
 *
 * ------------------------------------------------------------------------------
 */

int main(int argc, char * argv[])
{
   // client control struct
   clientctrl_t clientctrl = {0};

   // contendra la direccion IP y el número de puerto local
   struct sockaddr_in my_addr;

   // thread variables
   pthread_t thread;    pthread_attr_t attr = {0};

   // set thread detatch
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);   
   
   int retvalue = 0, i = 0;
   char exitkey = 0;


   // ---> check arguments
      // too few arguments
      if(argc != 3)
      {
         fprintf(stderr,"Use: %s [port] [mpu6050 parameter]\n\n", argv [0]);
         exit(1);
      }

      // get port and devconf from command line
      clientctrl.serverport = atoi(argv[1]);
      clientctrl.driverconfig = atoi(argv[2]);


   // ignore the PIPE signal
   // need to avoid the program close when this signal arrives
   signal(SIGPIPE, SIG_IGN);


   // open listen-socket
   if((clientctrl.sockfd = open_conection(&my_addr, &clientctrl)) == -1)
   {
      printf("[ERROR] Cannot open connection\n"); 
      exit(1);
   }


   // welcome message
   printf("\
         -------------------------------------------------------\n\
         --                       Server                     ---\n\
         -------------------------------------------------------\n");
   printf("Waiting for connections...\n");


   // create accept thread
   if((retvalue = pthread_create(&thread, &attr, server_accept, 
                                 &clientctrl)) != 0)
   {
      fprintf(stderr,"[ERROR] Thread creation error. ERROR CODE: %d\n", retvalue);
      exit(1);
   }


   // ---> wait for 'x' key
      while(exitkey != 'x')
      {
         printf("\n\
         -------------------------------------------------------\n\
         -- To close this program, must enter: 'x' + 'ENTER'  --\n\
         -------------------------------------------------------\n");

         // wait for a key to close
         scanf("%c", &exitkey);
      }


   // ---> close all servers
      printf("\n\
         -------------------------------------------------------\n\
         --                 Closing connections              ---\n\
         -------------------------------------------------------\n");

      printf("Closing clients connection...\n\n");

      // semaphore release
      sem_destroy(&clientctrl.sem_mutex);
      // shared memory release
      shmdt(clientctrl.shm_pointer);
      // IPC_RMID : Mark the segment to be destroyed
      shmctl(clientctrl.shm_id, IPC_RMID, NULL);           


      if(clientctrl.count == 0)
      {
         printf("There are no clients connected\n\n", i);
      }
      else
      {
         for(i = 0; i < clientctrl.count; i++)
         {
            if(clientctrl.status[i] == NOTCONNECTED)
            {
               printf("[%d] Client connection already close\n", i);
            }
            else
            {
               close(clientctrl.fd[i]);
               printf("[%d] Client connection close\n", i);
            }
         }
      }


   // must kill all threads  
   exit(0);
}



