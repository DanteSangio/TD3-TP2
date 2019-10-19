// argumentos variables
#include <stdarg.h> // oermite el uso de argumentos variables
/* por ejemplo printf en base a los % que vé se fija cuantos argumentos tendrá*/
/* al compilar agregar -Wall para ver todos los warning*/
#include <stdio.h>
#include <stdlib.h>

	void func(int num,...);

int main(int argc, char const *argv[])
/* los argumentos son siempre contemplando 1 extra, que es la dir del ejecutable*/
/* */
{
	// argv (char **)
	// argv[i] ((char *))
	//
	// argv[0] ruta del ejecutable
	// argv[1] primer argumento

	if(argc != 6)
	{
		printf("USO: ./arg_var_prom num1 num2 num3 num4 num5 \n Cuente bien caramba \n");
		return(32);
		// este ret lo levanto con ------> echo $?
	}

	//promedio de 5 valores argc-1
	func((argc-1), atoi(argv[1]),atoi(argv[2]),atoi(argv[3]),atoi(argv[4]),atoi(argv[5]) );
	//aun recibiendo 5, promedia 3(que le indico), pero es la misma función
	func((argc-3), atoi(argv[1]),atoi(argv[2]),atoi(argv[3]));
	return(0);//podria ser exit
}

	void func(int num,...)
	{
		va_list args;
		int arg=0;
		int prom=0;
		int i;

		va_start(args,num);//hasta num argumentos

		for(i=0;i<num;i++)
		{
			arg+=va_arg(args,int);// cada uno lo defini como int
		}
		
		va_end(args);//para indicar que ya recibi todo
		prom=arg/num;
		printf(" el promedio es %d, de %d valores\n",prom,num );
	}