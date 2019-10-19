/** 
   \file    client.c   
   \version 01.00
   \brief   Main file of the TCP/IP client.
            - This program needs some arguments to work. These will be entered in
            the command line, as IP, PORT and frequency of each server
            - Here the TCP/IP socket is created and binded to one thread
            - It creates one thread for each server
            - To close the client, you must enter 'x' + 'intro'

   \author  Agustin Diaz Antuna
   \date    09/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
#include "client.h"


/* ------------------------------------------------------------------------------
 * --- main
 * ------------------------------------------------------------------------------
 * @fn         main
 *
 * @brief      Main process of the TCP/IP client
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
   // socket file descriptor
   int sockfd = 0;

   // server variables
   serverctrl_t server[1000];
   int i = 0,  j = 0,   servercount = argc / 3;
   datatoserver_t datatoserver = {0};
   char exitkey = 0;

   // return value of read/write
   int status = 0;

   // thread struct
   pthread_t thread;    pthread_attr_t attr = {0};
   int retvalue = 0;

   // set thread detatch
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);


   // ---> check arguments
      // too few arguments
      if(argc < 2)
      {
         fprintf(stderr,"Use: %s [ip1] [port1] [frequency1] [ip2] [port2] [frequency2]...\n\n", argv [0]);
         exit(1);
      }

      // check for incomplete secuence (ip + port + frequency)
      if((argc - 1) % 3)
      {
         fprintf(stderr,"Incomplete arguments! Must be [ip] [port] [frequency]\n\n");
         exit(1);
      }


   // ---> ignore the PIPE signal
      // need to avoid the program close when this signal arrives
      signal(SIGPIPE, SIG_IGN);


   // ---> connect to each server
      printf("\n\
         -------------------------------------------------------\n\
         --                       Client                     ---\n\
         -------------------------------------------------------\n");

      printf("Connecting to servers...\n\n");

      // to create one thread for each server
      for(i = 1; i < argc; i+=3, j++)
      {
         server[j].number = j;

         strncpy(server[j].ip, argv[i], strlen(argv[i]) + 1);
         strncpy(server[j].port, argv[i+1], strlen(argv[i+1]) + 1);

         // 0  1  2  3  4  5  6  7  8  9
         // p  i  po f  i  po f  i  po f 
         // set connection to server -> blocking function
         // server_ipport[0] = ip
         // server_ipport[1] = port
         sockfd = conectar(&server[j]);

         if(sockfd < 1)
         {
            server[j].state = NOTCONNECTED;
            printf("[%d] Couldn't connect to server %s:%s\n\n", j, server[j].ip, server[j].port);
         }
         else
         {
            server[j].state = CONNECTED;
            server[j].fd = sockfd;
            server[j].frequency = atoi(argv[i + 2]);

            printf("[%d] Connected to server %s:%s\n", j, server[j].ip, server[j].port);
            printf("[%d] Data frequency = %d\n\n", j, server[j].frequency);

            // create thread
            if((retvalue = pthread_create(&thread, &attr, client_request, &server[j])) != 0)
            {
               fprintf(stderr,"---> [ERROR] Thread creation error. ERROR CODE: %d\n", retvalue);
               exit(1);
            }
         }
      }


   // ---> wait for 'x' key
      while(exitkey != 'x')
      {
         printf("\n\
         -------------------------------------------------------\n\
         -- To close this program, must enter: 'x' + 'ENTER'  --\n\
         -------------------------------------------------------\n\n");

         // wait for a key to close
         scanf("%c", &exitkey);
      }


   // ---> close all servers
      printf("\n\
         -------------------------------------------------------\n\
         --                 Closing connections              ---\n\
         -------------------------------------------------------\n");

      printf("Closing servers connection...\n\n");


      for(i = 0; i < servercount; i++)
      {
         if(server[i].state == NOTCONNECTED)
         {
            printf("[%d] Server %s:%s already close\n", i, server[i].ip, server[i].port);
         }
         else
         {
            datatoserver.status = GOODBYE_SERVER;

            write(server[i].fd, &datatoserver, sizeof(datatoserver));

            close(server[i].fd);
            printf("[%d] Server %s:%s connection close\n", i, server[i].ip, server[i].port);
         }
      }


   // must kill all threads
   exit(0);
}



