#include <sys/types.h>  // pid_t
#include <sys/wait.h>   // waitpid()
#include <stdio.h>      // printf()

#include "header.h"     // main_t st

void sigchld_handler(int sig)
{
    pid_t pid_to_wait = -1;  // espera cualquier pid
    pid_t stat_pid;
    int status;

    #ifdef DEBUG_CHILD
        static int handler_entries = 0, while_entries = 0;
        handler_entries++;
    #endif

	do
    {
        #ifdef DEBUG_CHILD
            while_entries++;
        #endif

	    stat_pid = waitpid (pid_to_wait, &status, WNOHANG | WUNTRACED | WCONTINUED);
        // WNOHANG      return immediately if no child has exited.
        // WUNTRACED    also  return  if  a  child  has stopped.
        // WCONTINUED   also return if a stopped child has been  resumed
        //              by delivery of SIGCONT.

	} while (stat_pid > 0 );

    #ifdef DEBUG_CHILD
        while_entries--;
        printf("handler_entries: %d; while_entries: %d\n", handler_entries, while_entries);
    #endif
}