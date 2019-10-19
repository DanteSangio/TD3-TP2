#include <unistd.h>     // getpid(), fork(), sleep()
#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // atoi(), malloc()

#include "header.h"     // main_t, RET_MAIN_ERROR_ARG, RET_MAIN_ERROR_FORK, RET_MAIN_OK


main_t st;

int main (int argc, char *argv[])
{
    int i;

    if (argc != 5)
    {
        printf ("USO: ./exe [num_proc] [num_threads] [time_proc] [time_threads]\n");

        return RET_MAIN_ERROR_ARG;
    }

    st.pid_dad = getpid();

    st.num_proc        = atoi (argv[1]);
    st.num_threads     = atoi (argv[2]);
    st.time_proc       = atoi (argv[3]);
    st.time_threads    = atoi (argv[4]);

    // Seteo handler para signal SIGCHLD
    st.new_action.sa_handler = sigchld_handler;
    sigemptyset (&st.new_action.sa_mask);
    st.new_action.sa_flags = SA_RESTART; // SA_RESTART: no se borra el handler para la signal

    if (sigaction (SIGCHLD, &st.new_action, &st.old_action) == -1)
    {
        perror("sigaction");
    }


    st.pid_childs = (pid_t *) malloc (sizeof (pid_t) * st.num_proc);


    for (i = 0; i < st.num_proc; i++)
    {
        st.pid_childs[i] = fork ();

        if (st.pid_childs[i] == -1)
        {
            perror("fork");

            return RET_MAIN_ERROR_FORK;
        }
        else if (st.pid_childs[i] == 0)
        {
            // Soy un hijo
            child_function(& st);

            return RET_MAIN_OK;
        }
        // soy el padre
    }

    printf("Esperando muerte de los hijos...\n");

    st.segundos = 0;

    while(st.segundos <= st.time_proc + 5 || st.segundos <= st.time_threads + 5)
    {
        sleep(1);
        printf("%d segundos\n", ++st.segundos);
    }

    free_all (& st);

    printf("Pulse ^C para terminar\n");

    while(1)
    {
        sleep(1);
    }

    return RET_MAIN_OK;
}