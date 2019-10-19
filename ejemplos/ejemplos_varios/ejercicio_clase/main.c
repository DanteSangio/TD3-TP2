#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/wait.h>

enum CODIGOS_SALIDA
{
	OK,
	ERROR_FORK
	ERROR_PARAMETROS
	ERROR_THREAD
};


int main(int argc, char const *argv[])
{
	int n_procesos=0;
	int n_hilos=0;		//hilos por proceso
	int tiempo_proceso=0;
	int tiempo_hilo=0;
	int i;

	// recibo parametros del main
	if (argc<5)
		{
			printf("parametros insuficientes\n USO: ./ejercicio n_procesos n_hilos tiempo_proceso tiempo_hilo\n");
			return(ERROR_PARAMETROS);
		}

	n_procesos=argv[1]-1;
	n_hilos=argv[2]-1;
	tiempo_proceso[3];
	tiempo_hilo[4];

	/************************************************/
	//creo los procesos

	pid_t pid_hijo[n_procesos];

		for (int i = 0; i < n_procesos; i++)
		{
			pid_hijo[i]=fork();
				/* en caso de error */
			if(pid_hijo[i] == -1)
				{
					// si tengo 2^16/2 procesos, me quede son memoria o no está soportado por la plataforma
					perror("error al crear el proceso n° %d:",i);
					return ERROR_FORK;
				}
		}



	// a partir de aca cada fork genera n hilos
	/************************************************/
	//creo los hilos
	//no creo n estructurasa dado que no uso ese identificador

	pthread_t mi_thread; //identificador sin uso
	pthread_attr_t *attr = NULL;
	int argumentos=tiempo_hilo;
	void *arg = &argumentos;
	int status;
	void* retval;

		for (int i = 0; i < n_hilos; i++)
		{
			status=pthread_create(& mi_thread,attr, incremento, arg);
			if(status != 0)
				{
					// si tengo 2^16/2 procesos, me quede son memoria o no está soportado por la plataforma
					perror("error al crear el hilo n° %d:",i);
					return ERROR_THREAD;
				}
		}



	return 0;
}

/************* FUNCIONES ******************/
void* incremento(void* arg)
{
	//printf("%d\n", ); //cuenta compartida
	return;
}

void sigchld_handler(int sig, siginfo_t siginfo, void* context)
{
	do{
	stat_pid=waitpid(-1,&status, WHOHAND | WUNTRACED | WCONTINUED);
	}while(stat_pid != (pid_t)0 && stat_pid != (pid_t)0 )

}
