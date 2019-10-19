/** 
   \file    cliente.c   
   \version 01.00
   \brief   Cliente
            - caracteristica 1
            - caracteristica 2

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
int main(int argc, char * argv[])
{
   /*File Descriptor para sockets*/
   int sockfd;
   /*Contendrá el número de bytes recibidos por read () */
   int numbytes;
   /* Buffer donde se reciben los datos de read ()*/
   char buf[MAXDATASIZE], bufEntrada[MAXDATASIZE], bufSalida[MAXDATASIZE];  


   /* Tratamiento de la línea de comandos. */
   if (argc < 2)
   {
      fprintf(stderr,"uso: %s hostname [port]\n",argv [0]);
      exit(1);
   }

   
   // socket file descriptor
   sockfd = conectar (argc, argv);
   

   while(1)
   {
      //Ingresamos el mensaje.
      gets(bufSalida);

      //Lo enviamos.
      if((numbytes = write(sockfd, bufSalida, MAXDATASIZE)) == -1)
      {
         perror("error de escritura en el socket");
         exit(1);
      }

      /* Recibimos los datos del servidor */
      if ((numbytes = read (sockfd, bufEntrada, MAXDATASIZE)) == -1)
      {
         perror("error de lectura en el socket");
         exit(1);
      }

      /* Visualizamos lo recibido */
      bufEntrada[numbytes] = '\0';
      printf("Recibido: %s\n",bufEntrada);
   }

   /* Devolvemos recursos al sistema */   
   close(sockfd);

   return 0;
}



