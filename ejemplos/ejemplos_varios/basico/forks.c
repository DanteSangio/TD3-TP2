#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

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
	pid_pibe=fork();
	return pid_pibe;
	// a partir de acá corren dos procesos IDENTICOS en simultaneo
	// pero no es practico, el hijo carga la variable con 0

	// LOS SEPARO POR CASOS

	/* en caso de error */
	if(pid_pibe == -1)
		{
			// si tengo 2^16/2 procesos, me quede son memoria o no está soportado por la plataforma
			perror("algo Malio Sal:");
			return ERROR_FORK;
		}

	/* proceso padre */
	else if(pid_pibe)
		{
			printf("Soy el padre y el pid del pibe es: %d\n", pid_pibe);
			sleep(10);
			//durante 5 seg nadie se enteró que el pibe termino de ejecutar
			//y no puede morir sin avisar
			// entonces ESTA ZOMBIE!!!
			//pid_muerto=wait(NULL);
			printf("finalmente murio el del pid %d\n",pid_muerto );
			// me entero de la muerte y entonces cagó fuego
			// el otro caso es cuando muere el padre sin darle wait (sepultura)
			// entonces lo sepulta el abuelo !!! PID=1 init
			sleep(10);
		}

		/* proceso padre */
	else
		{
			printf("Soy el hijo y mi PID es %d\n", getpid() );
		}

	return OK;
}

// probar con dos filtro