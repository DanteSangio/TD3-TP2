/** 
   \file    socklib_client.c   
   \version 01.00
   \brief   Conjunto de funciones de manejo de sockets desde el cliente
            - caracteristica 1
            - caracteristica 2

   \author  Agustin Diaz Antuna - Basado en: ejemplo CATEDRA INFORMATICA 1
   \date    09/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
#include "socklib_client.h"


/* ------------------------------------------------------------------------------
 * --- conectar
 * ------------------------------------------------------------------------------
 *
 * @fn         conectar
 *
 * @brief      Set client's connection to one server. This function does:
 *             - Load IP (string format) into struct sockaddr_in
 *             - Open socket
 *             - Load struct sockaddr_in
 *             - Connect cliente to server through socket
 *
 * @param [in] ip_string[0:1] char pointer with server IP:PORT
 *
 * @return     sockfd         file descriptor from the new socket
 *
 ********************************************************************************/
int conectar(serverctrl_t * server)
{
   // Sock file descriptor
   int sockfd;
   // Se utiliza para convertir el nombre del host a su direccion IP
   struct hostent *he;
   // Direccion del server donde se conectara
   struct sockaddr_in server_addr;  


   // Convertimos el nombre del host a su direccion IP
   if((he = gethostbyname((const char *) server->ip)) == NULL)
   {
      printf("---> Error: gethostbyname <---\n");
      exit(1);
   }


   // Creamos el socket
   if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
      printf("---> Error: Creating socket <---\n");
      exit(1);
   }


   // Establecemos server_addr con la direccion del server
   server_addr.sin_family = AF_INET;
   server_addr.sin_port = htons(atoi(server->port));
   server_addr.sin_addr = *((struct in_addr *)he->h_addr);
   bzero(&(server_addr.sin_zero), 8);


   // Intentamos conectarnos con el servidor
   if(connect(sockfd, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1)
   {
      printf("---> Error: Trying to connect to server <---\n");
      return NOTCONNECTED;
   }


   return sockfd;
}



