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

	fd = open("/dev/Dan", O_RDWR);

	write(fd, "msg", 1);
	read(fd, msg, 5);
	close(fd);
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