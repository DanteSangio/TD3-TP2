#ifndef _SERVER_H
#define _SERVER_H

/********************INCLUDES********************/

#include <stdio.h>
#include <unistd.h>        // open() close()
#include <stdlib.h>        // atoi()
#include <arpa/inet.h>     // inet_ntoa()
#include <semaphore.h>     // semaphore
#include <sys/shm.h>       // shared memory
#include <string.h>        // strerror()
#include <pthread.h>       // thread
#include <fcntl.h>         // open()
#include <sys/ioctl.h>     // ioctl()
#include <time.h>          // time()
#include <sys/wait.h>      // signal()

/******************FIN INCLUDES******************/

/********************PLANTILLA HTTP********************/

const char gszHttpTemplate[] = {"HTTP/1.1 200 OK \
Date: Mon, 27 Jul 2009 12:28:53 GMT \
Server: Apache/2.2.14 (Win32) \
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT \
Content-Length: %lu \
Content-Type: text/html \
Connection: Closed \
\n\n \
%s"};

/********************FIN PLANTILLA HTTP*****************/

/********************PLANTILLA PAGINA WEB********************/

const char gszPageTemplate[] = {
"<!DOCTYPE html> \
<html> \
<head> \
    <meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" /> \
    <meta charset=\"utf-8\" /> \
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\"> \
    <title>UTN FRBA - Trabajo Práctico Técnicas Digitales 3 - 2do Cuatrimestre</title> \
    <link href=\"https://fonts.googleapis.com/css?family=Roboto\" rel=\"stylesheet\"> \
    <link href=\"https://fonts.googleapis.com/css?family=Roboto:100\" rel=\"stylesheet\"> \
    <link href=\"https://fonts.googleapis.com/css?family=Roboto:200\" rel=\"stylesheet\"> \
    <link href=\"https://fonts.googleapis.com/css?family=Roboto:300\" rel=\"stylesheet\"> \
    <link href=\"https://fonts.googleapis.com/css?family=Roboto:400\" rel=\"stylesheet\"> \
    <link href=\"https://fonts.googleapis.com/css?family=Roboto:500\" rel=\"stylesheet\"> \
    <link href=\"https://fonts.googleapis.com/css?family=Roboto:600\" rel=\"stylesheet\"> \
    <link href=\"https://fonts.googleapis.com/css?family=Roboto:700\" rel=\"stylesheet\"> \
    <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/css/bootstrap.min.css\" \
        integrity=\"sha384-Gn5384xqQ1aoWXA+058RXPxPg6fy4IWvTNh0E263XmFcJlSAwiGgFAW/dAiS6JXm\" crossorigin=\"anonymous\"> \
    <style> \
        .measure-box { \
            border-radius: 10px; \
            padding: 20px 20px 20px 20px; \
            background-color: #cccccc; \
        } \
    </style> \
</head> \
<body> \
    <div class=\"row\"> \
        <div class=\"container\"> \
            <div class=\"jumbotron\"> \
                <h2>UTN FRBA - Técnicas Digitales 3 - Trabajo 2do cuatrimestre</h2> \
                <p> \
                    Alumno: %s \
                    Legajo: %s \
                </p> \
            </div> \
            <label class=\"alert alert-success\">La última medición exitosa fue tomada: %s</label> \
            <div class=\"row\"> \
                <div class=\"col-4\"> \
                </div> \
                <div class=\"col-4 measure-box text-center\"> \
                    <h4>Valor de medición:</h4> \
                    <h5>%s</h5> \
                </div> \
                <div class=\"col-4\"> \
                </div> \
            </div> \
        </div> \
        <script src=\"https://code.jquery.com/jquery-3.2.1.slim.min.js\" \
            integrity=\"sha384-KJ3o2DKtIkvYIK3UENzmM7KCkRr/rE9/Qpg6aAZGJwFDMVNA/GpGFF93hXpG5KkN\" \
            crossorigin=\"anonymous\"></script> \
        <script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.12.9/umd/popper.min.js\" \
            integrity=\"sha384-ApNbgh9B+Y1QKtv3Rn7W3mgPxhU9K/ScQsAP7hUibX39j7fakFPskvXusvfa0b4Q\" \
            crossorigin=\"anonymous\"></script> \
        <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/4.0.0/js/bootstrap.min.js\" \
            integrity=\"sha384-JZR6Spejh4U02d8jOt6vLEHfe/JQGiRRSQQxSfFWpi1MquVdAyjUar5+76PVCmYl\" \
            crossorigin=\"anonymous\"></script> \
</body> \
</html>"
};

/********************FIN PLANTILLA PAGINA WEB*****************/

#endif /*_SERVER_H*/