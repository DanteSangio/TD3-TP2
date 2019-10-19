#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(void)
{
	int fd;
	char msg[10];

   printf("Abriendo driver...\n");

	fd = open("/dev/Dan", O_RDWR);

   if (fd != 0)
   {
      printf("Driver abierto.\n");

      printf("Escribiendo driver...\n");
      write(fd, "msg", 1);
      printf("Driver escrito.\n");

      printf("Leyendo driver...\n");
      read(fd, msg, 5);
      printf("Driver leido.\n");

      printf("Cerrando driver...\n");
      close(fd);
      printf("Driver cerrado.\n");
   }
   else 
   {
      printf("¡¡¡Driver no se pudo abrir!!!\n");
   }

   printf("Saliendo\n");
	exit(1);
}

/*
int main(void)
{
   int num;

   FILE *fptr;

   fptr = fopen("/dev/Dan","rw");

   if(fptr == NULL)
   {
      printf("Error!");   
      exit(1);             
   }

   printf("Enter num: ");

   scanf("%d",&num);

   fprintf(fptr,"%d",num);

   fclose(fptr);

   return 0;
}
*/