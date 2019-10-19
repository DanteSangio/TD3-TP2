#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <time.h>

#define MYPORT 3490    // puerto donde se conectaran los clientes
#define BACKLOG 2      // Tamaño de la cola de conexiones recibidas

char buff[80];
pid_t pid, pid_hijo;
int hijos=3;                     // cantidad de procesos concurrentes

int EnviaHora (int socket);
void HandCLD(int);

int main() {
   int sockfd;      // El servidor escuchara por sockfd
   int newfd;	     // las transferencias de datos se realizar por aqui
   
   struct sockaddr_in my_addr;	 // IP y el numero de puerto local
   struct sockaddr_in their_addr; // IP y numero de puerto del cliente
   unsigned int sin_size;	       // tamaño de la escructura sockaddr_in
   
   pid=getpid();
   
   // Crea un socket y verifica si hubo algun error
   if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
   {
	  perror("socket");
	  exit(1);
   }
   
   // Asignamos valores a la estructura my_addr 
   // para luego poder llamar a la funcion bind()
   my_addr.sin_family = AF_INET;
   my_addr.sin_port = htons(MYPORT); // debe convertirse a network 
   my_addr.sin_addr.s_addr = inet_addr("0.0.0.0");	
   bzero(&(my_addr.sin_zero), 8);	 // rellena con ceros el resto 
   
   if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1)
   {
	  perror("bind");
	  exit(1);
   }
   
   // Habilitamos el socket para recibir conexiones
   printf("[%d] Server escuchando en: %s puerto %d\n", 
      pid, inet_ntoa(my_addr.sin_addr), MYPORT);
   if (listen(sockfd, BACKLOG) == -1)
   {
	  perror("listen");
	  exit(1);
   }
   
   // Instalamos el manejador de señales para saber que terminó.
   signal(SIGCLD,HandCLD);
   
   // loop que llama a accept()
   while(1)	{
	   sin_size = sizeof(struct sockaddr_in);
	   // Se espera por conexiones
      newfd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
      if(hijos) {
         hijos --;
         if (!(pid_hijo=fork()))
            {// Aca comienza el proceso hijo
            EnviaHora(newfd);
            close(newfd);
            close(sockfd);
            return(0);
            }
         else {
            printf ("[%d] Conexion desde: %s:%d, disponibles: %d, atiende: %d\n", 
               pid, inet_ntoa(their_addr.sin_addr),
               ntohs(their_addr.sin_port), hijos, pid_hijo);
            close(newfd);
            }
         }
      else {
         // Excedio la cantidad de concurrentes
         bzero(buff, sizeof(buff));
         sprintf(buff, "[%d] Lo siento no hay capacidad pruebe mas tarde :(\n", pid);
         send(newfd, buff, strlen(buff), 0);
         close(newfd);
         printf ("[%d] Conexion desde: %s:%d RECHAZADA, disponibles: %d\n", 
            pid, inet_ntoa(their_addr.sin_addr), 
            ntohs(their_addr.sin_port), hijos);
         }
	  }
   close(sockfd);
   }

int EnviaHora (int socket) {
   time_t t;
   pid= getpid();
   bzero(buff, sizeof(buff));
   sprintf(buff, "[%d] Hello, world! (ENTER para salir) \n", pid);
   send(socket, buff, strlen(buff), 0);
   while((recv(socket, buff, sizeof(buff), MSG_DONTWAIT))==-1) {
      sleep(1);
      time(&t);
      bzero(buff, sizeof(buff));
      sprintf(buff, "[%d] Es el dia %s\r", pid, ctime(&t));
      send(socket, buff, strlen(buff), 0);
      }
   return(0);
   }

void HandCLD(int signal) {
   hijos ++;
   pid_hijo=wait(NULL);
   printf("[%d] Proceso hijo %d finalizo, conexiones disponibles: %d \n", 
      pid, pid_hijo, hijos);
   }