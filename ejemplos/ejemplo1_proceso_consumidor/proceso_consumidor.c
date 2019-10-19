/**
\file proceso_consumidor.c
\author Mario Maqueda mariomaqueda@hotmail.com
\date 13 de agosto de 2015
\version 1.0
*/


#include "proceso_consumidor.h"

int main(void)
{
    sem_t *data_ready = NULL, *refill = NULL;
    key_t keyCam;
    char * MemoriaCompartida;
    int id_MemoriaCompartida;
    char ruta[MAX_RUTA], c;
    int errno_guardado;
    bool salir = false;

    IplImage *imagen = NULL;

    /************************************************************************************************************************/

    imagen = cvCreateImage( cvSize( ANCHO_IMAGEN, ALTO_IMAGEN ), IPL_DEPTH_8U, 3 );
    if (!imagen)
    {
        fprintf(stderr, "[error] Consumidor: no se pudo obtener un cuadro despues de setear el tamaño de la captura \n");
        return -3;
    }
    cvStartWindowThread();

    cvNamedWindow("Consumidor", CV_WINDOW_AUTOSIZE);


    /************************************************************************************************************************/

    sprintf(ruta, "%s", DATA_READY_TYPE);
    data_ready = sem_open(ruta, O_CREAT, 0644, 0);
    if(data_ready == SEM_FAILED)
    {
        fprintf(stderr, "[error] no se pudo abrir el semaforo %s para la memoria compartida: ", ruta);
        return -10;
    }

    sprintf(ruta, "%s", REFILL_TYPE);
    refill = sem_open(ruta, O_CREAT, 0644, 0);
    if(refill == SEM_FAILED)
    {
        fprintf(stderr, "[error] no se pudo abrir el semaforo %s para la memoria compartida: ", ruta);
        return -10;
    }


    /************************************************************************************************************************/

    if(crear_archivo_memoria_compartida() != 0)
    {
        fprintf(stderr, "[error] Consumidor: no se pudo crear el archivo para la memoria compartida \n");
        return -4;
    }

    sprintf(ruta, "%s%s%s", RUTA_MEMORIA_COMPARTIDA, NOMBRE_MEMORIA_COMPARTIDA, EXTENSION_MEMORIA_COMPARTIDA);
    if( (keyCam = ftok(ruta, 0)) == -1 )
    {
        errno_guardado = errno;
        fprintf(stderr, "[error] Consumidor: (ftok) no se pudo abrir el archivo %s para la memoria compartida: ", ruta);
        errno = errno_guardado;
        perror(NULL);

        return -4;
    }

    if( (id_MemoriaCompartida = shmget(keyCam, (int)imagen->imageSize, 0666 | IPC_CREAT)) == -1 )
    {
        errno_guardado = errno;
        fprintf(stderr, "[error] Consumidor: (shmget) no se pudo abrir la memoria compartida %s: ", ruta);
        errno = errno_guardado;
        perror(NULL);

        return -5;
    }

    // Me uno a la memoria compartida
    MemoriaCompartida = (char *)shmat(id_MemoriaCompartida,(void *)0,0);
    if(MemoriaCompartida == (char *)(-1))
    {
        errno_guardado = errno;
        fprintf(stderr, "[error] Consumidor: (shmat) no se pudo abrir la memoria compartida %s: ", ruta);
        errno = errno_guardado;
        perror(NULL);

        return -6;
    }

    fprintf(stderr, "[info] proceso Consumidor: memoria compartida abierta \n");


    /************************************************************************************************************************/

    while(!salir)
    {
        fprintf(stderr, "\ningrese \"S\" para pedir una imagen, \"P\" para salir: ");
        c = getchar();
        __fpurge(stdin);
        if (c == 'P' || c == 'p')
            salir = true;

        else if (c == 'S' || c == 's' )
        {
            if (sem_post(refill) == -1)
            {
                perror("Error en sem_wait(mutex) \n");
                return -8;
            }

            fprintf(stdout, "[info] Consumidor: Esperando imagen... \n");

            if (sem_wait(data_ready) == -1)
            {
                perror("Error en sem_wait(mutex) \n");
                return -8;
            }

            memcpy((char *)imagen->imageData, (char *)MemoriaCompartida, imagen->imageSize);

            cvShowImage("Consumidor", imagen);
            cvWaitKey(1);
        }

    }

    sem_close(refill);
    sem_close(data_ready);
    cvDestroyWindow("Consumidor");
    return 0;
}


int crear_archivo_memoria_compartida()
{
    char ruta[MAX_RUTA];
    FILE *tmp_file = NULL;

    sprintf(ruta, "%s%s%s", RUTA_MEMORIA_COMPARTIDA, NOMBRE_MEMORIA_COMPARTIDA, EXTENSION_MEMORIA_COMPARTIDA);
    if((tmp_file = fopen(ruta, "w+")) == NULL)
    {
        fprintf(stderr, "[error] controlar_camaras: no se pudo abrir el archivo para la memoria compartida %s: ", ruta);
        return -11;
    }
    fprintf(tmp_file, "A"); // se guarda algo en el archivo
    if((fclose(tmp_file)) != 0)
    {
        fprintf(stderr, "[error] controlar_camaras: no se pudo cerrar el archivo para la memoria compartida %s: ", ruta);
        return -12;
    }
    return 0;
}
