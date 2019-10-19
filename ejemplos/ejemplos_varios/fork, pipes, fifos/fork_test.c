#include <unistd.h>
#include <stdlib.h> //para exit
#include <stdio.h>	//para perror
#include <sys/types.h>// getpid
#include <sys/wait.h>
#include <signal.h>

	pid_t pid;      //pid_t = int
	int status;

/* respecto a funcion signal child*/
void sigchld_hand(int senial)
{
	//wait(NULL);
	int p;
	p=waitpid(-1,&status,WNOHANG); 
}

int main()
{
	int i=0;
	pid_t pid_padre = getpid();

	/* instalo el handler de signal */
	if(signal(SIGCHLD, sigchld_hand) == SIG_ERR)
		{
			perror("signal:");
			exit(2);
		}
	pid = fork();   // no recibe arg, -1 si salio algo mal y no crea proceso

	if(pid == -1) 
		{
			perror("fork:");//con los : respeto el error que me dé
			exit(1);
		}
	/* funciono y tengo el proceso hijo */
	else if(pid == 0)
		{
			printf("soy child. Mi pid es %d \n", getpid()); //el padre nunca pasa por aca
			sleep(60);
		}
	else
		{
			printf("Soy el padre de %d ,y mi pid es %d \n", pid, getpid() ); // el hijo nuca pasa por aca
			//sleep(60);
			//wait(0);
			sleep(60);
		}
	exit(0);
}



/*
Se compila con: gcc -o fork_test fork_test.c
El proceso crea al hijo, termina con el padre y luego atiende al hijo

fork creo la instancia que apunta a la misma memoria fisica, entonces, mismo codigo y datos.
Compartir la seccion TEXT evita la copia de codigo innecesaria.
Los datos son distintos para procesos distintos, de hecho la variable PID la ven de distinto valor
Copy on Write (COW):
	Se trata de usas la menor cantidad de memoria para lograr porcesos livianos, en el momento que un
	proceso quiera modificar una variable, el SO la duplica.
	Por ejemplo, si la pagina era read only, y por defecto lo es, en la page fault se hace el COW,
	que en realidad duplica la pagina, la cual es writable y la otra cambio a writable tambien.

Al finalizar el padre, el hijo finaliza tambien.
Sleep puso a dormir al padre, 60 es la cantidad de segundos que lo pone a dormir =>quedo zombi el hijo.
El proceso padre espera la finalizacion de sus hijos, para esto uso wait(), al llamarla se pone sleeping,
entonces esperará 60 segundos al hijo.
Para resolverlo correctamente seria interesante no usar el wait, esto se hará a travez de señales.
	La señal, señala eventos de un proceso a otro de forma Casi asincronica.
	POSIX tiene 31 señales, linux agrega 32 más. La 17 SIGCHLD, envia al proceso padre una señal de finalización.
	Con signal le paso el numero de la señal y que funcion atendera el evento.
	Las señales pueden ser guardadas como pendientes en el process descriptor de tocarle a otra tarea o lo que fuera.
	Este metodo no contempla más que un hijo con el wait null.
	Salvo kill y stop, podemos cambiar las eñales o ignorarlas, por ejemplo CTRL-C.
*/