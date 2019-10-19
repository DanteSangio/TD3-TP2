#include <signal.h>     // sigaction(), SIGCHLD
#include <time.h>       // time()
#include <stdlib.h>     // rand(), srand()
#include <unistd.h>     // usleep()
#include <stdio.h>      // perror()
#include <pthread.h>    // pthread_create(), pthread_t

#include "header.h"

void child_function(main_t *st)
{
    int i;
    int time_start = time(NULL);
    int time_elapsed;
    pthread_t thread_id;

    // vuelvo a poner la señal a como estaba originalmente en el hijo
    if (sigaction (SIGCHLD, &st->old_action, NULL) == -1)
    {
        perror("sigaction");
    }

    for (i = 0; i < st->num_threads; i++)
    {
        pthread_create(&thread_id, NULL, thread_function, st);
    }

    time_elapsed = time(NULL) - time_start;
    sleep(st->time_proc - time_elapsed);
}

void *thread_function(void *arg)
{
    main_t *st = arg;

    sleep(st->time_threads);

    return NULL;
}