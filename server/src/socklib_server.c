/** 
   \file    socklib_server.c   
   \version 01.00
   \brief   Conjunto de funciones de manejo de sockets desde el servidor
            - caracteristica 1
            - caracteristica 2

   \author  Agustin Diaz Antuna - Basado en: ejemplo CATEDRA INFORMATICA 1
   \date    09/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
#include "socklib_server.h"


/* ------------------------------------------------------------------------------
 * --- open_conection
 * ------------------------------------------------------------------------------
 *
 * @fn         open_conection
 *
 * @brief      Open server's socket. This function does:
 *             - Open socket
 *             - Load struct sockaddr_in
 *  				- Bind socket and struct sockaddr_in
 *             - Set maximum length to the queue of pending connections
 *
 * @param [in] server_addr	struct sockaddr_in
 *
 * @return     sockfd      file descriptor from the new socket
 *
 ********************************************************************************/
int open_conection(struct sockaddr_in * server_addr, clientctrl_t * clientctrl)
{
	// socket file descriptor
	int sockfd;
	// variable auxiliar
	int aux;


	// Crea un socket y verifica si hubo algun error
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{
		fprintf(stderr, "---> Error in function socket. ERROR CODE: %s\n <---", strerror(sockfd));
		exit(1);
	}


	// Asignamos valores a la estructura server_addr familia de sockets INET
	// para UNIX
	server_addr->sin_family = AF_INET;
	// convierte el entero formato PC a entero formato network
	server_addr->sin_port = htons(clientctrl->serverport);
	// automaticamente usa la IP local
	server_addr->sin_addr.s_addr = INADDR_ANY;
	// rellena con ceros el resto de la estructura
	bzero(&(server_addr->sin_zero), 8);


	// Con la estructura sockaddr_in completa, se declara en el sistema que este 
	// proceso escuchara pedidos por la IP y el port definidos
	if((aux = bind(sockfd, (struct sockaddr *) server_addr, sizeof(struct sockaddr))) == -1)
	{
		fprintf(stderr, "---> Error in function bind. ERROR CODE: %s\n <---", strerror(aux));
		exit(1);
	}


	// Habilitamos el socket para recibir conexiones, con una cola de conexiones 
	// en espera que tendra como maximo el tamano especificado en BACKLOG
	if((aux = listen(sockfd, BACKLOG)) == -1)
	{
		fprintf(stderr, "---> Error in function listen. ERROR CODE: %s\n <---", strerror(aux));
		exit(1);
	}
	

	return sockfd;
}




/* ------------------------------------------------------------------------------
 * --- aceptar_pedidos
 * ------------------------------------------------------------------------------
 *
 * @fn         aceptar_pedidos
 *
 * @brief      Wait for new client connection. Blocking function. This function
 *					does:
 *             - Call accept()
 *             - Creates a new socket to the incoming client conection
 *
 * @param [in] sockfd 	file descriptor from the listening socket
 *
 * @return     newfd    file descriptor from the new socket
 *					-1			when error
 *
 ********************************************************************************/
int aceptar_pedidos(int sockfd)
{
	// New sock fd
	int newfd;
	// stuct to save client's IP:PORT
	struct sockaddr_in client_addr;
	unsigned int sin_size = sizeof(struct sockaddr_in);


	// Wait for new connections
	if((newfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1)
	{
		fprintf(stderr, "---> Error in function accept. ERROR CODE: %s\n <---", strerror(newfd));
		exit(1);
	}
	else
	{
		printf("\nNew connection from: %s\n", inet_ntoa(client_addr.sin_addr));
		return newfd;
	}
}



