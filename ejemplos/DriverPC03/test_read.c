/** 
   \file    02_test_read.c   
   \version 01.00
   \brief   Driver basico de Linux
            - caracteristica 1
            - caracteristica 2

   \author  Agustin Diaz Antuna
   \date    23/10/2018
   \modif   Dante Sangiovanni
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


/* ------------------------------------------------------------------------------
 * --- define
 * ------------------------------------------------------------------------------
 */
#define MAX_BUFFER  4


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
   int fd, read_size = 0;
   char buffer[MAX_BUFFER];

   // open driver
   fd = open("/dev/Dan", O_RDONLY);
   if(fd < 0)
   {
      printf("Error de open.\n");
      return 1;
   }

   printf("Escriba enter para leer\n");
   getchar();

   read_size = read(fd, buffer, MAX_BUFFER);

   if(read_size < 0)
   {
      printf("Error de lectura\n");
      close(fd);
      return 1;
   }
   
   printf("Lo leido es: %s\n", buffer);

   getchar();

   close(fd);

   return 0;
}







