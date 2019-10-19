/** 
   \file    02_test_write.c   
   \version 01.00
   \brief   Driver basico de Linux
            - caracteristica 1
            - caracteristica 2

   \author  Agustin Diaz Antuna
   \date    23/10/2018
*/

/* ------------------------------------------------------------------------------
 * --- include
 * ------------------------------------------------------------------------------
 */
#include "stdio.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "string.h"


/* ------------------------------------------------------------------------------
 * --- define
 * ------------------------------------------------------------------------------
 */
#define MAX_BUFFER  256


/* ------------------------------------------------------------------------------
 * --- struct
 * ------------------------------------------------------------------------------
 */



/* ------------------------------------------------------------------------------
 * --- variables
 * ------------------------------------------------------------------------------
 */


/* ------------------------------------------------------------------------------
 * --- functions
 * ------------------------------------------------------------------------------
 */
int main(void)
{
   int fd, write_size = 0;
   char buffer[MAX_BUFFER];
   bzero(buffer, MAX_BUFFER);

   // open driver
   fd = open("/dev/mi_drv", O_WRONLY);

   if(fd < 0)
   {
      printf("Error de open.");
      return 1;
   }

   printf("Escriba un mensaje:\n");
   fgets(buffer, MAX_BUFFER, stdin);

   buffer[4] = '\0';

   printf("Buffer: a%sa\n", buffer);

   write_size = write(fd, buffer, strlen(buffer));

   if(write_size < 0)
   {
      close(fd);
      printf("Error de escritura, con write_size: %d\n", write_size);
      return 1;
   }
   
   printf("Escritura correcta.\n");

   getchar();

   close(fd);

   return 0;
}







