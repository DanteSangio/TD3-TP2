/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <stdio.h>
#include <stdlib.h>
////////#include <sys/sockio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <unistd.h>
#include <server.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CANT_BUFF 20

/*

Vamos a leer un archivo de configuración -> OK!

Vamos a leer una propiedad del device tree -> OK!

Vamos a probar leer el driver instalado -> OK!

Vamos a agregar hilos para tener multiples conexiones

*/

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    /*Variables asociadas al driver*/
    int fd;
    int read_size = 0;
    char bufferRead[CANT_BUFF];

    /*Variables asociadas al device-tree*/
    FILE* fiddtcom;
    FILE* fiddtmod;
    FILE* fiddtnam;

    /*Variables asociadas al archivo de configuracion*/
    FILE* fid;
    char cadena[25];
    char simbolo;
    int cant = 20;
    int cont = 0;
    int num;
    int backlog;
    int activas;
    int frecuencia;
    int media; 

    /*Variables asociadas al servidor*/
    socklen_t clilen;
    int sockfd, newsockfd, portno;
    char buffer[100000];
    char outBuffer[100000];
    char pageBuffer[100000];
    struct sockaddr_in serv_addr, cli_addr;
    int n;

    if (argc < 2)
    {
        fprintf(stderr, "ERROR, no se seleccionó un puerto\n");
        exit(1);
    }

    /*Driver*/
    printf("\nLeyendo el driver...\n");

    fd = open("/dev/Dan", O_RDONLY);
    if(fd < 0)
    {
      printf("Error de open.\n");
      return 1;
    }

    printf("Escriba enter para leer\n");
    getchar();

    read_size = read(fd, bufferRead, CANT_BUFF);

    if(read_size < 0)
    {
      printf("Error de lectura\n");
      close(fd);
      return 1;
    }

    printf("Lo leido es: %s\n", bufferRead);

    getchar();

    close(fd);

    printf("\nDriver cerrado...\n");

    /*Device-Tree*/
    printf("\nLeyendo el device-tree...\n");

    /*Device-Tree Compatible*/
    fiddtcom=fopen("/proc/device-tree/compatible","r");

    if (fiddtcom==NULL)
    {
        printf("Problema al leer device-tree\n");
        exit(-1);
    }
    if (fgets(cadena, 20, fiddtcom) != NULL)
        printf("\nLa propiedad compatible es: %s\n", cadena);

    fclose(fiddtcom);

    /*Device-Tree Model*/
    fiddtmod=fopen("/proc/device-tree/model","r");

    if (fiddtmod==NULL)
    {
        printf("Problema al leer device-tree\n");
        exit(-1);
    }
    if (fgets(cadena, 20, fiddtmod) != NULL)
        printf("\nLa propiedad model es: %s\n", cadena);

    fclose(fiddtmod);

    /*Device-Tree Name*/
    fiddtmod=fopen("/proc/device-tree/name","r");

    if (fiddtmod==NULL)
    {
        printf("Problema al leer device-tree\n");
        exit(-1);
    }
    if (fgets(cadena, 20, fiddtmod) != NULL)
        printf("\nLa propiedad name es: %s\n", cadena);

    printf("\nFin de la lectura del device-tree...\n");

    fclose(fiddtmod);

    /*Archivo de configuracion*/
    fid=fopen("../inc/config.cfg","r");

    if (fid==NULL)
    {
        printf("Problema al leer el archivo de configuracion\n");
        exit(-1);
    }

    printf("\nLeyendo el archivo de configuracion...\n");

    while((simbolo = fgetc(fid)) != EOF)
    {
        if ( simbolo == '=' )
        {
            cont++;

            if (fgets(cadena, 20, fid) == NULL)
                break;

            num = atoi( cadena );

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
    printf("\nLa configuracion de backlog es: %d\n", backlog);
    printf("\nLa configuracion de conexiones activas es: %d\n", activas);
    printf("\nLa configuracion de frecuencia es: %d\n", frecuencia);
    printf("\nLa configuracion de calculo de media es: %d\n", media);

    fclose(fid);

    printf("\nArchivo de configuracion cerrado.\n");

    printf("\nCreando servidor...\n");

    /*Servidor*/
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    bzero((char *)&serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR on binding");

    listen(sockfd, 5);

    clilen = sizeof(cli_addr);

    printf("\nEsperando conexiones...\n");

    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);

    if (newsockfd < 0)
        error("ERROR on accept");

    bzero(buffer, sizeof(buffer));

    n = read(newsockfd, buffer, sizeof(buffer));

    if (n < 0)
        error("ERROR reading from socket\n");

    printf("Here is the message:\n %s\n", buffer);

    //itoa(backlog, cadena, 10);
    sprintf(pageBuffer, gszPageTemplate, "Sangiovanni, Dante | ", "149.167-2", "15:35", cadena);
    
    //sprintf(pageBuffer, gszPageTemplate, "backlog: %d", "activas: %d", "frecuencia: %d", "media: %d", backlog, activas, frecuencia, media);
    sprintf(outBuffer, gszHttpTemplate, strlen(pageBuffer), pageBuffer);

    n = write(newsockfd, outBuffer, strlen(outBuffer));

    if (n < 0)
        error("ERROR writing to socket\n");

    printf("Bytes sent: %d.\n Message was: %s\n", n, outBuffer);

    close(newsockfd);

    close(sockfd);

    return 0;
}