#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

/*########################################################*/

#define PORTtcp 8145
/*########################################################*/

void* func_th(void*);
/*########################################################*/

int fdSocket, fdCliente;

char mensaje_bienvenida[]="Conexión aceptada";

struct sockaddr_in ServerTcp, ClienteTcp;
socklen_t tamSocket;

int nth=0;
/*########################################################*/
/*########################################################*/
/*########################################################*/

int main (void)
{

	//==========================================
	pthread_t mi_thread; //estructura
	pthread_attr_t *attr = NULL;
	int argumento = 7;
	void* arg = &argumento;
	int status;
	void* retval;
	//==========================================

	fdSocket=socket(PF_INET,SOCK_STREAM,0);
	memset(&ServerTcp,0,sizeof(ServerTcp));
	ServerTcp.sin_family=AF_INET;
	ServerTcp.sin_port=htons(PORTtcp);
	ServerTcp.sin_addr.s_addr=INADDR_ANY;

	tamSocket=sizeof(ServerTcp);

	// vincula proceso con el puerto
	bind(fdSocket,(struct sockaddr*)&ServerTcp,tamSocket);
	// crea la cola de espera
	listen(fdSocket,1000);

	//==========================================
	// inicio del servicio
	//==========================================
	while(1)
	{
		printf("existen %d hilos vivos\n", nth);
		fdCliente=accept(fdSocket,(struct sockaddr*)&ClienteTcp,&tamSocket);
		pthread_create(& mi_thread,attr, func_th, arg);
	}

}

/*########################################################*/

void* func_th(void* arg)
{
	char recibido[20];
	nth++;
	void* ret=&nth;
		printf("%s\n",mensaje_bienvenida);
		read(fdCliente,recibido,20);
		printf("mensaje recibido: \"%s\"\n soy el hilo %d\n\n",recibido,nth);
		write(fdCliente,"recibido",strlen("recibido")+1);
		sleep(10);
		nth--;
		printf("existen %d hilos vivos\n", nth);

	return ret;

}