#include	<stdio.h>
#include	<stdlib.h>
#include	<unistd.h>
#include	<strings.h>
#include	<signal.h>
#include	<sys/select.h>
#include	<sys/types.h>
#include	<errno.h>
#include	<sys/socket.h>
#include	<sys/wait.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<netdb.h>
#include    <pthread.h>
#include <sys/shm.h>



// SON DOS PROCESOS DISTINTOS !!!
enum CODIGOS_SALIDA
{
	OK,
	ERROR_FORK
};


int main(int argc, char const *argv[])
{
	/* code */
	pid_t pid_pibe,pid_muerto;
	key_t key;
	int shmid;
	int *data;
	data=0;
	key = ftok("/home/beej/algo",'R');
	shmid = shmget(key,1024,0644| IPC_CREAT);
	pid_pibe=fork();
	return pid_pibe;
	//---------------------------------------------------------------

	if(pid_pibe == -1)
		{
			perror("algo Malio Sal:");
			return ERROR_FORK;
		}

	/* proceso padre */
	else if(pid_pibe)
		{
			printf("Soy el padre y el pid del pibe es: %d\n, y data vale %d\n", pid_pibe,*(int*)data);
			data = shmat(shmid, (void *)0, 0);
			data++;
			sleep(10);
			printf("finalmente murio el del pid %d\n",pid_muerto );
		}

	/* proceso padre */
	else
		{
			data = shmat(shmid, (void *)0, 0);	
			data++;		
			printf("Soy el hijo y mi PID es %d\n y data vale %d\n", getpid(),*(int*)data);
		}

	return OK;
}
