#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char const *argv[])
{
	FILE* fid;

	fid=fopen("/proc/device-tree/compatible","r");

	if (fid==NULL)
	{
		printf("Problema al leer device-tree\n");
		exit(-1);
	}

	printf("\nLeyendo el device-tree...\n\n");

	printf("\nFin de la lectura del device-tree...\n\n");

	fclose(fid);

	return 0;
}