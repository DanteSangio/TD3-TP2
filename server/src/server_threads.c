/** 
   \file    server_threads.c   
   \version 01.00
   \brief   This file contains the threads created on the TCP/IP server
            - server_accept
            - td3_i2c_handler
            - client_request_handler
            - client_timeout

   \author  Agustin Diaz Antuna
   \date    27/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
#include "server.h"


/* ------------------------------------------------------------------------------
 * --- server_accept
 * ------------------------------------------------------------------------------
 * @fn         server_accept
 *
 * @brief      This thread is blocked until a new client try to connect.
 *             - On the first client's connection, creates an exclusive thread to
 *             handle the driver
 *             - Also creates one thread for every new client
 *
 * @param      void * arg     pointer to a clientctrl_t struct
 *
 * @return     void *
 *
 * ------------------------------------------------------------------------------
 */

void * server_accept(void * arg)
{
   // server control struct pointer
   clientctrl_t * clientctrl = ((clientctrl_t *) arg);

   // thread variables
   pthread_t thread;    pthread_attr_t attr = {0};
   int retvalue = 0;

   // set thread detatch
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);   

   int i = 0, td3_i2c_handler_status = 0;


   while(1)
   {
      // blocking function to wait for new connections
      clientctrl->fd[i] = aceptar_pedidos(clientctrl->sockfd);


      if(td3_i2c_handler_status == 0)
      {
         // ****************************************************
         // create td3 i2c handler thread
         if((retvalue = pthread_create(&thread, &attr, td3_i2c_handler, 
                                       clientctrl)) != 0)
         {
            fprintf(stderr,"Thread creation error. ERROR CODE: %d\n", retvalue);
            exit(1);
         }

         td3_i2c_handler_status = 1;
      }


      // ****************************************************
      // create client request thread
      printf("[%d] Client number = %d\n", i, i);
      clientctrl->status[i] = CONNECTED;
      i++;

      if((retvalue = pthread_create(&thread, &attr, client_request_handler, 
                                    clientctrl)) != 0)
      {
         fprintf(stderr,"Thread creation error. ERROR CODE: %d\n", retvalue);
         exit(1);
      }
   }

   return 0;
}




/* ------------------------------------------------------------------------------
 * --- td3_i2c_handler
 * ------------------------------------------------------------------------------
 * @fn         td3_i2c_handler
 *
 * @brief      This is the only thread that can interact with the driver.
 *             - Use the file operations open/close/read/write/ioctl to acquire
 *             data from the MPU6050 and writes it on a global buffer every one
 *             second
 *
 * @param      void * arg     pointer to a clientctrl_t struct
 *
 * @return     void *
 *
 * ------------------------------------------------------------------------------
 */

void * td3_i2c_handler(void * arg)
{
   // clients control struct pointer
   clientctrl_t * clientctrl = ((clientctrl_t *) arg);

   // driver
   int td3_i2c_fd = 0;
   char datatodriver = 0;
   int16_t datafromdriver = 0;

   int i = 0, status = NOTCONNECTED, prevstatus = NOTCONNECTED;


   // initialize semaphore
      // 2nd arg: 0 -> is shared between the threads of a process
      // 3rd arg: 1 -> init value
      sem_init(&clientctrl->sem_mutex, 0, 1);

   // initialize shared memory
      // creates shared memory
      clientctrl->shm_id = shmget(IPC_PRIVATE, sizeof(datatoclient_t), 0666|IPC_CREAT);
      // attach the shared memory segment identified by shmid 
      // to the address space of the calling process
      clientctrl->shm_pointer = (datatoclient_t*) shmat(clientctrl->shm_id, NULL, 0);




   while(1)
   {
      sleep(SLEEP_TD3_I2C);


      prevstatus = status;
      status = NOTCONNECTED;

      
      // ****************************************************
      // checking if all the clients are gone
      for(i = 0; i <= clientctrl->count; i++)
      {
         if(clientctrl->status[i] != NOTCONNECTED)
            status = CONNECTED;
      }


      if((prevstatus == NOTCONNECTED) && (status == CONNECTED))
      {
         // ****************************************************
         // opening driver
         printf("Opening driver...\n\n");
         if((td3_i2c_fd = open(DRIVER_PATH, O_RDWR)) < 0)
         {
            printf("Cannot open driver\n\n");
            exit(1);
         }   
         printf("Driver open successfully!\n\n");
      }
      
      if((prevstatus == CONNECTED) && (status == NOTCONNECTED))
      {
         // ****************************************************
         // closing driver
         printf("Closing driver...\n\n");
         close(td3_i2c_fd);
         printf("Driver closed successfully!\n\n");
      }
      
      if(status == CONNECTED)
      {
         // take semaphore
         sem_wait(&clientctrl->sem_mutex);

         
         // ****************************************************
         // reading driver
         // axis -> x
         if((clientctrl->driverconfig == 1) || (clientctrl->driverconfig == 4))
         {
            datatodriver = MPU6050_ACCEL_X;
            write(td3_i2c_fd, (char *) &datatodriver, sizeof(char));

            read(td3_i2c_fd, &datafromdriver, sizeof(datafromdriver));
            clientctrl->shm_pointer->accel[0] = (float)(datafromdriver) * ARES_2G;
         }

         // axis -> y
         if((clientctrl->driverconfig == 2) || (clientctrl->driverconfig == 4))
         {
            datatodriver = MPU6050_ACCEL_Y;
            write(td3_i2c_fd, (char *) &datatodriver, sizeof(char));

            read(td3_i2c_fd, &datafromdriver, sizeof(datafromdriver));
            clientctrl->shm_pointer->accel[1] = (float)(datafromdriver) * ARES_2G;
         }

         // axis -> z
         if((clientctrl->driverconfig == 3) || (clientctrl->driverconfig == 4))
         {
            datatodriver = MPU6050_ACCEL_Z;
            write(td3_i2c_fd, (char *) &datatodriver, sizeof(char));

            read(td3_i2c_fd, &datafromdriver, sizeof(datafromdriver));
            clientctrl->shm_pointer->accel[2] = (float)(datafromdriver) * ARES_2G;
         }


         // ****************************************************
         // ioctling driver
         ioctl(td3_i2c_fd, IOCTL_CANT, &clientctrl->shm_pointer->read_count);


         // release semaphore
         sem_post(&clientctrl->sem_mutex);
      }
   }

   return 0;
}




/* ------------------------------------------------------------------------------
 * --- client_request_handler
 * ------------------------------------------------------------------------------
 * @fn         client_request_handler
 *
 * @brief      This thread handle the client request. There is one of this threads
 *             for each client.
 *             - It receives the data frequency
 *             - Creates the timeout (i-am-alive) thread for each client
 *             - Waits the data period and send the data to the client
 *
 * @param      void * arg     pointer to a clientctrl_t struct
 *
 * @return     void *
 *
 * ------------------------------------------------------------------------------
 */

void * client_request_handler(void * arg)
{
   // clients control struct pointer
   clientctrl_t * clientctrl = ((clientctrl_t *) arg);

   // client request handler number
   // each thread has its own number
   int number = clientctrl->count++;
   int status = 0, frequency = 0, count = 0;

   // driver
   char datatodriver = 0;
   int16_t datafromdriver = 0;

   // data to client
   datatoclient_t datatoclient = {0};
   struct tm *tm;
   
   // thread variables
   pthread_t thread;    pthread_attr_t attr = {0};
   int retvalue = 0;

   // set thread detatch
   pthread_attr_init(&attr);
   pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);   


   // receive client frequency
   if((status = read(clientctrl->fd[number], &frequency, sizeof(frequency))) < 1)
   {
      printf("Error reading socket\n");
      exit(1);
   }

   printf("[%d] Data frequency = %d\n\n", number, frequency);




   // create client timeout thread
   if((retvalue = pthread_create(&thread, &attr, client_timeout, clientctrl)) != 0)
   {
      fprintf(stderr,"[ERROR] Thread creation error. ERROR CODE: %d\n", retvalue);
      exit(1);
   }




   while(1)
   {
      sleep(frequency);

      // ****************************************************
      // updating time
      time(&datatoclient.timer);
      tm = localtime(&datatoclient.timer);


      // ****************************************************
      // data to client
      // take semaphore
      sem_wait(&clientctrl->sem_mutex);

      datatoclient.accel[0] = clientctrl->shm_pointer->accel[0];
      datatoclient.accel[1] = clientctrl->shm_pointer->accel[1];
      datatoclient.accel[2] = clientctrl->shm_pointer->accel[2];
      
      count = clientctrl->shm_pointer->read_count;
      
      // release semaphore
      sem_post(&clientctrl->sem_mutex);

      // ****************************************************
      // sending data to client
      if((status = write(clientctrl->fd[number], &datatoclient, 
                        sizeof(datatoclient))) < 1)
      {
         printf("---> Writing socket error <---\n");
         
         clientctrl->status[number] = NOTCONNECTED;
         close(clientctrl->fd[number]);

         printf("[%d] Client number %d is disconnected\n\n", number, number);

         return 0;
      }


      // ****************************************************
      // printing data
      printf("------------------------------------------\n");
      printf("-- [%d] Data to client number %d \t\t--\n", number, number);
      printf("------------------------------------------\n");
      printf("-- Date: %d-%d-%d -- Time: %d:%d:%d\t--\n", tm->tm_year + 1900, 
            tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
      printf("------------------------------------------\n");
      // printf("bytesread: %d\n", bytesread);
      if((clientctrl->driverconfig == 1) || (clientctrl->driverconfig == 4))
         printf("-- Accelerometer AXIS X = %f\t--\n", datatoclient.accel[0]);
      if((clientctrl->driverconfig == 2) || (clientctrl->driverconfig == 4))
         printf("-- Accelerometer AXIS Y = %f\t--\n", datatoclient.accel[1]);
      if((clientctrl->driverconfig == 3) || (clientctrl->driverconfig == 4))
         printf("-- Accelerometer AXIS Z = %f\t--\n", datatoclient.accel[2]);
      printf("------------------------------------------\n");
      printf("-- Number of lectures since insmod: %d\t--\n", count);
      printf("------------------------------------------\n\n");
   }

   return 0;
}




/* ------------------------------------------------------------------------------
 * --- client_timeout
 * ------------------------------------------------------------------------------
 * @fn         client_timeout
 *
 * @brief      This thread must check for the i-am-alive client's message.
 *                - MAX_CLIENT_TIMEOUT = 10 seconds
 *             - There is one of this threads for every new client
 *             - Waits the timeout period and reads the client's data
 *
 * @param      void * arg     pointer to a clientctrl_t struct
 *
 * @return     void *
 *
 * ------------------------------------------------------------------------------
 */

void * client_timeout(void * arg)
{
   // server control struct pointer
   clientctrl_t * clientctrl = ((clientctrl_t *) arg);
   datafromclient_t datafromclient;

   // client request handler number
   // each thread has its own number
   int number = clientctrl->count - 1;

   // make a nonblocking read()
   int flags = fcntl(clientctrl->fd[number], F_GETFL, 0);
   fcntl(clientctrl->fd[number], F_SETFL, flags | O_NONBLOCK);


   while(1)
   {
      // client timeout
      sleep(MAX_CLIENT_TIMEOUT);


      // read client data
      read(clientctrl->fd[number], &datafromclient.status, 
            sizeof(datafromclient.status));


      // check data from client
      if((datafromclient.status == GOODBYE_SERVER) || 
         ((datafromclient.status != I_AM_ALIVE) && (datafromclient.status != GOODBYE_SERVER)) ||
         (clientctrl->status[number] == NOTCONNECTED))
      {
         clientctrl->status[number] = NOTCONNECTED;
         close(clientctrl->fd[number]);
         return 0;
      }
   }

   return 0;
}



