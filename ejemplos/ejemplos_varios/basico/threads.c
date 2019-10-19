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
};

int dev=8;
void* func_th(void*);

int main(int argc, char const *argv[])
{
	/*int pthread_create(pthread_t *thread,				 ->estructura
						 const pthread_attr_t *attr,	 ->atributos
                         void *(*start_routine) (void *),->comienzo de la funcion a ejecutar
                         								   recibe y devuelve direcciones 
                         								   es un puntero a funcion
                         void *arg);  					 ->argumentos a la funcion

                         devuelve el estado		*/

	pthread_t mi_thread; //estructura
	pthread_attr_t *attr = NULL;
	int argumento = 7;
	void* arg = &argumento;
	int status;
	void* retval;

	pthread_create(& mi_thread,attr, func_th, arg);

	sleep(1);//sin esperar capaz que el sch no va a la funcion nunca y termina
	// con esto puedo ver el estado del thread, por ejemplo si aun corre
	// pero la funcion join es bloqueante
	status=pthread_join(mi_thread, &retval);
	printf("soy la funcion main, y el status es %d y el retorno es %d\n",status,*(int*)retval);
	// en el mismo proceso corre el thread y el main
	status=pthread_join(mi_thread, &retval);
	printf("soy la funcion main, y el status es %d y el retorno es %d\n",status,*(int*)retval);
	// en este segundo caso, me lo informa con un 3, o sea que el thread ya termino
	// en el mismo proceso corre el thread y el main

	return OK;
}

void* func_th(void* arg)
{
	void* ret=&dev;
	sleep(2);
	printf("soy la funcion del thread, y me pasaron el argumento %d\n",*((int*)arg));
	return ret;
	// NULL es un void*, de hecho:
	// #define NULL (void *) 0
}


