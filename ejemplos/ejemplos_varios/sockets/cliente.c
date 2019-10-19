#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>

void die(char * msg);

int main(int argc, char *argv[]) {
   int sockfd;
   struct sockaddr_in their_addr;  /* IP y puerto del servidor */
   int numbytes, puerto;
   char ack[20];
   if (argc != 4) {
      printf("uso: 6.4.cliente hostname puerto mensaje\n");
      exit(0);
      }

   // Creamos el socket
   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) die("socket");

   // Cargamos la estructura
   their_addr.sin_family = AF_INET;  /* host byte order */
   puerto = atoi(argv[2]);
   their_addr.sin_port = htons(puerto);  /* network byte order */
   their_addr.sin_addr.s_addr = inet_addr(argv[1]);
   bzero(&(their_addr.sin_zero), 8);

   if (connect(sockfd, (struct sockaddr*) &their_addr, sizeof(struct sockaddr)) == -1)
   {
      perror("in the oven");

      exit(-1);
   }


   // Enviamos el mensaje
   if ((numbytes=write(sockfd, argv[3], strlen(argv[3])+1)) == -1) die ("sendto");
   printf("enviados %d bytes hacia %s\n",numbytes, inet_ntoa(their_addr.sin_addr));
   read(sockfd,ack,20);
   printf("%s\n",ack );
   close(sockfd);
   return 0;
}

void die(char * msg) {
   char texto[80];
   sprintf(texto, "[%d] %s", getpid(), msg);
   perror(texto);
   exit(EXIT_FAILURE);
   }