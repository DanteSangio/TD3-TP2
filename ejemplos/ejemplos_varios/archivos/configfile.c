#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	FILE* fid;

	char cadena[20];
	char simbolo;
	int cant = 20;
	int cont = 0;
	int num;

	int backlog;
	int activas;
	int frecuencia;
	int media; 

	fid=fopen("config.cfg","r");

	if (fid==NULL)
	{
		printf("Problema al leer el archivo de configuracion\n");
		exit(-1);
	}

	printf("\nLeyendo el archivo de configuracion...\n\n");

	while((simbolo = fgetc(fid)) != EOF)
	{
		if ( simbolo == '=' )
		{
			cont++;

			printf("Simbolo igual encontrado\n");

			fgets(cadena, 20, fid);
			printf("Cadena capturada: %s\n", cadena);

			num = atoi( cadena );
			printf("Numero tranformado: %d\n", num);

			switch ( cont )
			{
				case 1: 
					backlog = num; 
					break;
				case 2: 
					activas = num; 
					break;
				case 3: 
					frecuencia = num; 
					break;
				case 4: 
					media = num; 
					break;
			}

			if (ferror( fid ) || feof( fid ))
                break;
		}
	}

	printf("\nEl resultado de la conversion fue el siguiente: \n\n");

	printf("\nbacklog: %d | activas: %d | frecuencia: %d | media: %d \n\n", backlog, activas, frecuencia, media);

	num = backlog + activas + frecuencia + media;

	printf("\nLa suma de todos los numeros es: %d\n\n", num);

	num /= 4;

	printf("\nEl promedio de los numeros es: %d\n\n", num);

	printf("\nFin de la lectura del archivo de configuracion...\n\n");

	fclose(fid);

	return 0;
}