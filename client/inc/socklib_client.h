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
   #include <stdlib.h>
   #include <unistd.h>
   #include <errno.h>
   #include <string.h>

   #include <sys/types.h>
   #include <sys/socket.h>
   #include <sys/wait.h>

   #include <arpa/inet.h>
   #include <netinet/in.h>
   #include <netdb.h>


/* ------------------------------------------------------------------------------
 * --- define
 * ------------------------------------------------------------------------------
 */
   // Para las transmisiones
   #define MAXDATABUFFER   256

   #define CONNECTED       1
   #define NOTCONNECTED    0

   // data to server -> status
   #define GOODBYE_SERVER  0
   #define I_AM_ALIVE      1   


/* ------------------------------------------------------------------------------
 * --- struct
 * ------------------------------------------------------------------------------
 */
   // servers control struct
   typedef struct {
      int      number;
      int      state;
      int      frequency;
      char     ip[10];
      char     port[7];
      int      fd;
   } serverctrl_t;

   // data to server
   typedef struct {
      int      frequency;
      int      status;
   } datatoserver_t;

   // data from server
   typedef struct {
      float    accel[3];
      time_t   timer;
      int      read_count;
   } datafromserver_t;



/* ------------------------------------------------------------------------------
 * --- prototipos
 * ------------------------------------------------------------------------------
 */

   // Funcion para abrir el socket y conectarse a determinado PORT:IP
   int conectar(serverctrl_t * server);



