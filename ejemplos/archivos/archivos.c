#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>


int main(int argc, char const *argv[])
{
	FILE* fid; //estructura
	char ip_read[20];
	int desde=0;
	int hasta=1;
	int cuenta=0;
	char todos=1;
	//==========================================
	if (argc>2)
	{
		todos=0;
		desde=atoi(argv[1]);
		hasta=atoi(argv[2]);
	}
	else if (argc==2)
	{
		todos=0;
		desde=atoi(argv[1]);
		hasta=desde;
	}
	//==========================================
	// apertura y chequeo
	fid=fopen("ips.txt","r");
	if (fid==NULL)
	{
		printf("no pudo leerse el archivo de configuracion\n");
		exit(-1);
	}
	
	//==========================================
	// recorro el archivo desde y hasta las posiciones indicadas
	while(fscanf(fid,"%s",ip_read) != EOF)
	{
		if (!todos)
		{
			cuenta++;
			if(cuenta==desde)
			{
				printf("ip inicial %d:%s\n",desde,ip_read);
			}
			else if (cuenta>desde && cuenta<=hasta)
			{
				printf("ip %d:%s\n",cuenta,ip_read);
			}
		}
		else
		{
			printf("%s\n",ip_read);
		}
	}

	//==========================================
	fclose(fid);
	return 0;
}