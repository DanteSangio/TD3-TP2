#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct my_msgbuf
{
	long mtype;//obligatorio
	char mtext[200];
};

int main(void)
{
	struct my_msgbuf buf;
	int msqid; //al crearla o conectarla, el SO devuelve un identificador
	key_t key; //clave de la cola

    /*ftok devuelve en key la clave, por cada combinacion da una clave,
    fallara si por ej, el .c no existe*/
	if((key = ftok("kirk.c",'B'))==-1)
	{
		perror("ftok");
		exit(1);
	}

	/* quiero tener acceso a la cola, con estos permisos, de mensaje con la clave key
	RW dueño, R para el grupo del dueño, R cualquier otro user, con la OR si no existe, la crea
	En msqid
	*/
	if (msqid=msgget(key,0644 | IPC_CREAT) ==-1)
	{
		perror("msgget");
		exit(1);
	}

	printf("Ingrese lineas de texto, ctrl+D para salir\n");//ctrl d es eof

		buf.mtype = 1;
		/* a gets le paso el puntero a estructura, mientras exista y no venga el fin de archivo...*/
	while(fgets(buf.mtext, sizeof buf.mtext, stdin) != NULL) 
	{
		if (msgsnd(msqid,(struct msgbuf *)&buf,sizeof(buf),0)== -1)
		//el flag 0, indica que de no haber mensajes para mi, no debo quedarme, para send con 0 estamos bien 
		{
			perror("msgsnd");//informo pero sigo
		}
	}

	/* antes de salir, la cola ya esta disponible, ctl, control, borra la cola , altera, modifica... */
	if (msgctl(msqid, IPC_RMID, NULL) == -1) 
	{
		perror("msgctl");
		exit(1);
	}

	return 0;
}				