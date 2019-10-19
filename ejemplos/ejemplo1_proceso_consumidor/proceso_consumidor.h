/**
\file proceso_consumidor.h
\author Mario Maqueda mariomaqueda@hotmail.com
\date 13 de agosto de 2015
\version 1.0
*/


#pragma once


#include <stdio.h>
#include <stdbool.h>
#include <semaphore.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <errno.h>
#include <stdio_ext.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>


#define ANCHO_IMAGEN 640
#define ALTO_IMAGEN 480

#define MAX_RUTA 200

#define RUTA_SEMAFORO ".././"
#define DATA_READY_TYPE "data_ready"
#define REFILL_TYPE "refill"

#define RUTA_MEMORIA_COMPARTIDA ".././"
#define NOMBRE_MEMORIA_COMPARTIDA "memoria_compartida"
#define EXTENSION_MEMORIA_COMPARTIDA ".txt"



int crear_archivo_memoria_compartida();
