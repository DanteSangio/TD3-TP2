/**
    @file       socket_tcp.c
    @author     Koremblum Nicolas Mariano (n.m.koremblum@ieee.org)
    @date       28/08/2018 (dd/mm/yyyy)
    @version    1.0
    @brief      ?
        
        Compilar con: gcc -o prueba -pthread main.c -Wall

    @todo       ?
**/

/* ------------------------------------------------------------ ;
;                                                               ;
;                           INCLUDE's                           ;
;                                                               ;
; ------------------------------------------------------------ */

#include <sys/types.h>

#include <sys/socket.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdio.h>

#include <unistd.h>

#include <errno.h>

#include <string.h>

#include <stdlib.h>

#include <netdb.h>

/* ------------------------------------------------------------ ;
;                                                               ;
;                           DEFINE's                            ;
;                                                               ;
; ------------------------------------------------------------ */

// Tamaño de la cola de conexiones recibidas
#define     TCP_BACKLOG             1000

#define     IS_ERROR(x)             (0 > x)

/* ------------------------------------------------------------ ;
;                                                               ;
;                           PROTOTYPE's                         ;
;                                                               ;
; ------------------------------------------------------------ */

int tcp_connect( char * server_ip, int server_port, \
                 struct sockaddr_in *clientAddrInfo);	


int tcp_create_connection(  int server_port, \
                            struct sockaddr_in * my_addr);


int tcp_accept_connection(  int sockFD, \
                            struct sockaddr_in * clientAddrInfo);


ssize_t tcp_send(int fd, void * data, int bytes);


ssize_t tcp_recv(int fd, void * data, int bytes);


/* ------------------------------------------------------------ ;
;                                                               ;
;                             CODE                              ;
;                                                               ;
; ------------------------------------------------------------ */

int tcp_connect (char * server_ip, int server_port, \
                                struct sockaddr_in *clientAddrInfo)
{
    int retval;
    int	sockFD;
    struct hostent *he;

    // Se convierte el nombre del host a su dirección IP
    if ((he = gethostbyname ((const char *) server_ip)) == NULL)
    {
        herror("Error en gethostbyname \n");
        return -1;
    }

    /* Creamos el socket */
    sockFD = socket(AF_INET, SOCK_STREAM, 0);
    
    if (IS_ERROR(sockFD))
    {
        perror("Error en creación de socket \n");
        return -1;
    }

    /* Establecemos clientAddrInfo con la direccion del server */
    clientAddrInfo->sin_family  = AF_INET;
    clientAddrInfo->sin_port    = htons(server_port);
    clientAddrInfo->sin_addr    = *((struct in_addr *)he->h_addr);
    bzero(&(clientAddrInfo->sin_zero), 8);

    /* Intentamos conectarnos con el servidor */
    retval = connect(sockFD, (  struct sockaddr *) clientAddrInfo, \
                                sizeof(struct sockaddr));

    if ( IS_ERROR(retval) )
    {
        perror("Error tratando de conectar al server \n");
        return -1;
    }

    return sockFD;
}

int tcp_create_connection ( int server_port, \
                            struct sockaddr_in * my_addr)
{
    int	sockaux;	/*socket auxiliar*/
    int	retval; 		/*variable auxiliar*/

    /*Crea un socket y verifica si hubo algún error*/
    sockaux = socket(AF_INET, SOCK_STREAM, 0);
    if ( IS_ERROR(sockaux) )
    {
        perror("\nSocket Error?: ");
        return -1;
    }

    /* Asignamos valores a la estructura my_addr */

    // familia de sockets INET para UNIX*/
    my_addr->sin_family = AF_INET;
    // Convierte el formato PC a formato Network
    my_addr->sin_port = htons(server_port);
    // Se setea automaticamente la IP local	
    my_addr->sin_addr.s_addr = INADDR_ANY;

    // Rellena con ceros el resto de la estructura
    bzero((my_addr->sin_zero), 8);

    /*  Con la estructura sockaddr_in completa, se declara en el 
        Sistema que este proceso escuchará pedidos por la IP y 
        el port definidos*/
    retval = bind(sockaux, (struct sockaddr *) my_addr, \
                                sizeof(struct sockaddr));
    if ( IS_ERROR(retval) )
    {
        perror("\nBind Error?: ");
        return -1;
    }

    /* 
        Se habilita el socket para recibir conexiones, con una cola
        de conexiones en espera que tendrá como máximo el tamaño 
        especificado en TCP_BACKLOG
    */
    retval = listen(sockaux, TCP_BACKLOG);

    if ( IS_ERROR(retval) )
    {
        perror("\nListen Error?: ");

        return -1;
    }
    
    return sockaux;
}

int tcp_accept_connection ( int sockFD, \
                            struct sockaddr_in * clientAddrInfo)
{
    // En este socket se clonará el socket principal
    int clonFD;

    unsigned int sinSize;

    // Se espera por conexiones (bloqueante)
    clonFD = accept(sockFD, (struct sockaddr *)clientAddrInfo, \
                                                        &sinSize);

    if ( IS_ERROR(clonFD) )     perror("\nAccept Error?: ");
    
    else    printf("[info] tcp_accept_connection(): %s\n", \
                                inet_ntoa(clientAddrInfo->sin_addr));
        
    return clonFD;
}

inline ssize_t tcp_send(int fd, void * data, int bytes)
{
    return send(fd, data, bytes, 0);
}

inline ssize_t tcp_recv(int fd, void * data, int bytes)
{
    return recv(fd, data, bytes, 0);
}