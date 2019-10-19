/** 
   \file    servidor.c   
   \version 01.00
   \brief   Servidor
            - caracteristica 1
            - caracteristica 2
            - IP (local) 127.0.0.1

   \author  Agustin Diaz Antuna
   \date    09/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
#include "sockets.h"
#include "sock-lib.h"


/* ------------------------------------------------------------------------------
 * --- define
 * ------------------------------------------------------------------------------
 */


/* ------------------------------------------------------------------------------
 * --- main
 * ------------------------------------------------------------------------------
 */
int main (void)
{
   /* File Descriptor del socket por el que el servidor "escuchará" conexiones*/
   int sockfd;
   char message[] = "Hello, world!";
   /* contendrá la dirección IP y el número de puerto local */
   struct sockaddr_in my_addr;
   int sockdup, numbytes; 


   if ((sockfd = Open_conection (&my_addr)) == -1)
   {
      perror ("Falló la creación de la conexión"); 
      exit (1);
   }

//   printf("IP: %\n");

   while(1)
   {
      sockdup = Aceptar_pedidos (sockfd);

      //Devuelve PID, el !PID = 0, entonces entra al if.   
      if(!fork())                      
      {
         while(1)
         {
            //Recibo mensaje.
            if ((numbytes = read (sockdup, message , sizeof (message))) == -1)
            {
               perror("Error recibiendo mensaje en socket");
               exit (1);
            }

            message[numbytes] = '\0';
//          sprintf(message,"Mensaje enviado desde el proceso %d.", getpid());

            if(strcmp(message,"fin"))
            {
               //Lo devuelvo.
               if (write (sockdup, message , strlen(message)) == -1)
               {
                  perror("Error escribiendo mensaje en socket");
                  exit (1);
               }
            }
            else
            {
               close(sockdup);
               exit(0);
            }

            close(sockdup);
            exit(0);
         }
      }

      close(sockdup);
   }
   
   exit(0);
}


