/**
\file proceso_productor.c
\author Mario Maqueda mariomaqueda@hotmail.com
\date 13 de agosto de 2015
\version 1.0
*/

#include "proceso_productor.h"

int main(void)
{

    sem_t *data_ready = NULL, *refill = NULL;

    key_t keyCam;
    char * MemoriaCompartida = NULL;
    int id_MemoriaCompartida;
    char ruta[MAX_RUTA];
    int i, valor_de_retorno;

    CvCapture* capture = NULL;
    IplImage *imagen = NULL;

    /************************************************************************************************************************/
    // Inicializacion de la imagen

    cvInitSystem(0, NULL);

    capture = cvCaptureFromCAM( -1 ); // Se abre la camara
    if (!capture)
    {
        fprintf(stderr, "[error] proceso productor: no se pudo abrir la camara \n");
        return -1;
    }

    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, ANCHO_IMAGEN);
    cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, ALTO_IMAGEN);

    imagen = cvQueryFrame( capture );
    if (!imagen)
    {
        fprintf(stderr, "[error] productor: no se pudo obtener la imagen \n");
        return -3;
    }

    cvStartWindowThread();
    cvNamedWindow("Productor", CV_WINDOW_AUTOSIZE);

    /************************************************************************************************************************/
    // Apertura de los semaforos

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
    // Creacion de la memoria compartida

    if(crear_archivo_memoria_compartida() != 0)
    {
        fprintf(stderr, "[error] Consumidor: no se pudo crear el archivo para la memoria compartida \n");
        return -4;
    }

    sprintf(ruta, "%s%s%s", RUTA_MEMORIA_COMPARTIDA, NOMBRE_MEMORIA_COMPARTIDA, EXTENSION_MEMORIA_COMPARTIDA);
    if( (keyCam = ftok(ruta, 0)) == -1 )
    {
        fprintf(stderr, "[error] productor: (ftok) no se pudo abrir el archivo %s para la memoria compartida: ", ruta);
        return -4;
    }

    if( (id_MemoriaCompartida = shmget(keyCam, (int)imagen->imageSize, 0666 | IPC_CREAT)) == -1 )
    {
        fprintf(stderr, "[error] productor: (shmget) no se pudo abrir la memoria compartida %s: ", ruta);
        return -5;
    }

    MemoriaCompartida = (char *)shmat(id_MemoriaCompartida,(void *)0,0);
    if(MemoriaCompartida == (char *)(-1))
    {
        fprintf(stderr, "[error] productor: (shmat) no se pudo abrir la memoria compartida %s: ", ruta);
        return -6;
    }

    /************************************************************************************************************************/
    // Bucle para tomar imagenes

    valor_de_retorno = 0;
    while(1)
    {
        fprintf(stdout, "[info] productor: esperando que se pida una imagen \n");
        if (sem_wait(refill) == -1)
        {
            perror("[error] productor: Error en sem_wait(refill) \n");
            valor_de_retorno = -8;
            break;
        }
            // Inicio Zona Critica ///////

            fprintf(stdout, "[info] productor: Tomando imagen... \n");
            for( i = 0 ; i < 10 ; i++ )
                if(! cvGrabFrame(capture))
                {
                    fprintf(stderr, "[error] productor: (cvGrabFrame) no se pudo tomar la imagen \n");
                    valor_de_retorno = -7;
                    break;
                }
            imagen = cvRetrieveFrame(capture, 0);
            if (!imagen)
            {
                fprintf(stderr, "[error] productor: (cvRetrieveFrame) no se pudo tomar la imagen \n");
                valor_de_retorno = -7;
                break;
            }

            memcpy((char *)MemoriaCompartida, (char *)imagen->imageData, imagen->imageSize);

            cvShowImage("Productor", imagen);
            cvWaitKey(5000); // demora para mostrar que se queda con los semaforos

            // Fin Zona Critica ///////

        if (sem_post(data_ready) == -1)
        {
            perror("[error] productor: Error en sem_post(data_ready) \n");
            valor_de_retorno = -8;
            break;
        }

    }

    sem_close(refill);
    sem_close(data_ready);
    cvDestroyWindow("Productor");
    return valor_de_retorno;
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
