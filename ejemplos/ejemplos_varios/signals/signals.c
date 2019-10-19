#include <signal.h>	// sigaction()
#include <stdio.h>	// printf()
#include <unistd.h>	// pause()
#include <stdlib.h>	// atoi()

void sigint_parent_handler(int signal);
void sigint_child_handler(int signal);


enum E_MAIN_RET
{
	RET_MAIN_OK,
	RET_MAIN_ERROR_NUM_ARGS,
	RET_MAIN_ERROR_SIGACTION,
};


int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		printf ("USO: ./exe [num_expected_signals]\n");

		return RET_MAIN_ERROR_NUM_ARGS;
	}

	struct sigaction sigaction_sigint;
	sigemptyset(&sigaction_sigint.sa_mask);

	int num_expected_signals = atoi(argv[1]);
	int i;
	if (fork() == 0)
	{
		//Soy el hijo

    	sigaction_sigint.sa_handler = sigint_child_handler;
	    if( sigaction(SIGINT, &sigaction_sigint, NULL) < 0 )
		{
			perror("\nSigaction error:");
			return RET_MAIN_ERROR_SIGACTION;
		}

		for (i = 0; i < num_expected_signals; i++)
		{
			printf("hijo: Waiting SIGINT...\n");
			pause();
		}
	}
	else
	{
		//Soy el padre

    	sigaction_sigint.sa_handler = sigint_parent_handler;
	    if( sigaction(SIGINT, &sigaction_sigint, NULL) < 0 )
		{
			perror("\nSigaction error:");
			return RET_MAIN_ERROR_SIGACTION;
		}

		for (i = 0; i < num_expected_signals; i++)
		{
			printf("padre: Waiting SIGINT...\n");
			pause();
		}
	}

	printf("Exiting program\n");

	return RET_MAIN_OK;
}


void sigint_parent_handler(int signal)
{
	static int num_recieved_signal = 0;
	num_recieved_signal++;

	printf("\npadre: SIGINT recieved %d time(s)\n", num_recieved_signal);
}


void sigint_child_handler(int signal)
{
	static int num_recieved_signal = 0;
	num_recieved_signal++;

	printf("\nhijo: SIGINT recieved %d time(s)\n", num_recieved_signal);
}
