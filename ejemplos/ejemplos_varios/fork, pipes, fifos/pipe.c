/*
conectores entre procesos: ps -elf | grep "R"
el | es este conector que redirige la salida de un
proceso a la entrada del otro.

ps -elf > pepe.txt 
redirige la salida al archivo

pipe es como una fifo con dos FD
*/

#include <unistd.h>
#include <stdlib.h> //para exit
#include <stdio.h>	//para perror

int main(int argc, char *argv[])
{
	int fp[2];

	if(argc!=2)
	{
		printf(stderr,"argumentos insuficientes\n");
		printf(stderr,"Uso %s NombreDelProceso.\n", argv[0]);
		exit(1);
	}

	pipe(fp);
	if(!fork())
	{
		dup2(fp[1],fileno(stdout)); //duplica la salida standar con fp
		close(fp[0]);//cierro el de lectura que no lo uso
		execpl("ps","ps","-elf",NULL);
	}
	else
	{
		up2(fp[0],fileno(stdin)); //duplica la salida standar con fp
		close(fp[1]);//cierra el de escritura por usar solo el de lectura
		execpl("grep","grep",argv[1],NULL);//reemplaza un proceso por la imagen de otro proceso
		// (binario a abrir, nombre para el SO, argumentos..., null para finalizar)
	}
}