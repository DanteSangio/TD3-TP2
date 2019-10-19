#include <signal.h>     // struct sigaction
#include <sys/types.h>  // pid_t

enum ret_main
{
    RET_MAIN_OK,
    RET_MAIN_ERROR_ARG,
    RET_MAIN_ERROR_FORK,
    RET_MAIN_ERROR_SHM,
    RET_MAIN_ERROR_MUTEX,
    RET_MAIN_ERROR_THREAD
};

typedef struct
{
    int num_proc, num_threads, time_proc, time_threads;
    pid_t *pid_childs;
    pid_t pid_dad;
    struct sigaction old_action;
    struct sigaction new_action;
    int segundos;
} main_t;

extern main_t st;


void free_all (main_t *st);
void sigchld_handler(int sig);
void child_function(main_t *st);
void *thread_function(void *);