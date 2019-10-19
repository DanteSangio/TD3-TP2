/** 
   \file    sock-lib.h   
   \version 01.00
   \brief   Archivos cabezera para las funciones de manejo de sockets brindados
            en Info1

   \author  Agustin Diaz Antuna
   \date    09/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
   #include <stdio.h>
   #include <unistd.h>        // open() close()
   #include <stdlib.h>        // atoi()
   #include <arpa/inet.h>     // inet_ntoa()
   #include <semaphore.h>     // semaphore
   #include <sys/shm.h>       // shared memory
   #include <string.h>        // strerror()


/* ------------------------------------------------------------------------------
 * --- define
 * ------------------------------------------------------------------------------
 */
   // Tamano de la cola de conexiones recibidas
   #define BACKLOG         2
   // Para las transmisiones
   #define MAXDATABUFFER   256

   // data to server -> status
   #define GOODBYE_SERVER  0
   #define I_AM_ALIVE      1

   // sleeps [ms]
   #define SLEEP_TD3_I2C      1
   #define MAX_CLIENT_TIMEOUT 10


/* ------------------------------------------------------------------------------
 * --- struct
 * ------------------------------------------------------------------------------
 */
   // data from client
   typedef struct {
      int      frequency;
      int      status;
   } datafromclient_t;


   // data to client
   typedef struct {
      float    accel[3];
      time_t   timer;
      int      read_count;
   } datatoclient_t;


   // clients control struct
   typedef struct {
      // server fields
      int               sockfd;
      uint16_t          serverport;
      // driver fields
      char              driverconfig;
      // client fields
      int               fd[1000];
      int               count;
      int               status[1000];
      // ipc
      sem_t             sem_mutex;
      int               shm_id;
      datatoclient_t    * shm_pointer;
   } clientctrl_t;


/* ------------------------------------------------------------------------------
 * --- prototipos
 * ------------------------------------------------------------------------------
 */

   // Funcion que crea la conexion
   int open_conection(struct sockaddr_in * server_addr, clientctrl_t * clientctrl);
   // Funcion que acepta una conexion entrante
   int aceptar_pedidos(int);



