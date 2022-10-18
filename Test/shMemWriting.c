#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

int main() // Add boolClassicWeekEnd in arg
{

	// Initialisation of variables
	int boolSprint = 0, boolClassicWeekEnd = 1, shmSize = sizeof(int)*2, shmId;
	int *shMem;

	// Get id for the sh m
	if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0775)) < 0)
	{
		perror("shmget error ");
		exit(EXIT_FAILURE);
	} 

	// For each child
    int pidFork;
    // Fork and check errors
    if ((pidFork = fork()) == -1)
    {
        perror("fork error");
        exit(EXIT_FAILURE);
    }

    // Get the sh m from the id
    if ((shMem = (int *) shmat(shmId, NULL, 0)) < 0)
    {
        perror("shmat error ");
        exit(EXIT_FAILURE);
    }

    // Child (a car)
    if (pidFork == 0)
    {	
        int data = 2;
        memcpy(&shMem[0], &data, sizeof(data));

        // sleep(1);
        printf("child : %d\n", shMem[0]);
        
        if((shmdt(shMem)) < 0)
        {
            perror("shmdt error ");
            exit(EXIT_FAILURE);
        }
        
        // Child have to not make another child
        exit(EXIT_SUCCESS);
    }

    int data = 4;

    memcpy(&shMem[1], &data, sizeof(data));

    printf("parent %d\n", shMem[1]);

    sleep(1);
	if((shmdt(shMem)) < 0)
	{
		perror("shmdt error ");
		exit(EXIT_FAILURE);
	}

    if((shmctl(shmId, IPC_RMID, 0)) > 0)
	{
		perror("shmctl error ");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}