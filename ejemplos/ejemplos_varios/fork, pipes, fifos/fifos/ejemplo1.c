#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#define hola "holaaaaaaaaaaaaaa\n\n"

int main(void)
{
	int file_d,cant;
	/* el pipe puede crearse desde consola con 
		mknod m 660 myfifo p
	   o desde el programa con
	    int myfifo(const char* path_carpeta, modo_t macro_modo)
	   cuyas macros estan en 
	    /home/agus/Software/matlab/sys/lcc/include/sys/stat.h
	*/
	//devuleve el file descriptor, si falla devuelve -1
	file_d = open("mi_pipe", O_WRONLY);
	if (file_d== -1)
	{
		perror("mi_pipe open");
		exit(1); // con 1 suele entenderse como que si no es 0 hubo un error
	}

	//lo abri para escribir pero nadie más lo pide, cuando hice cat
	//lo leyo, entonces mi programa encontró el lector
	//printf("%d\n",file_d); //el 0 es in, 1out, 2 error, entonces este es el 3 	
	cant=write(file_d,hola,strlen(hola)*sizeof(char));
	//escribimos en el pipe y cat lo leyó
	sleep(10);
	close(file_d);
	exit(0);
	//sleep(10);//va a hacer esperar 10 seg a cat



}

/*
lo creo en el shell con
mrfifo -m 660 mi_pipe
*/