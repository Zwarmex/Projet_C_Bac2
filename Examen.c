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
#include "Headers/Functions.h"

int main() // Add boolClassicWeekEnd in arg
{

	// Initialisation of variables
	int boolSprint = 0, boolClassicWeekEnd = 1, shmSize = sizeof(int) * NUMBEROFCARS, shmData[NUMBEROFCARS], shmId,
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55};//, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};

	int *shMem;

	if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0775)) < 0)
	{
		perror("shmget error ");
		exit(EXIT_FAILURE);
	} 
	
	if ((shMem = (int *) shmat(shmId, NULL, 0)) < 0)
	{
		perror("shmat error ");
		exit(EXIT_FAILURE);
	}

	// Create array of cars
	Car *arrayCars = CarBuilder(arrayCarsId);

	// For each child
	for (int i = 0; i < NUMBEROFCARS; i++)
	{

		int pidFork;
		// Fork and check errors
		if ((pidFork = fork()) == -1)
        {
			perror("fork error");
            exit(EXIT_FAILURE);
		}
	
		// Put seed number in rand with pid of the processus
		srand(time(NULL) ^ (getpid()<<16));

		// Child (a car)
		if (pidFork == 0)
        {	
			if (boolClassicWeekEnd)
			{
				printf("\nFriday's morning : Free Try\n\n");
				DoFreeTry(&arrayCars[i]);
				printf("\nFriday's afternoon : Free Try\n\n");
				DoFreeTry(&arrayCars[i]);
				printf("\nSaturday's morning : Free Try\n\n");
				DoFreeTry(&arrayCars[i]);
				memcpy(&shMem[i], &arrayCars[i].totalTurnMS, sizeof(arrayCars[i].totalTurnMS));
			}	
			// Child have to not make another child
			exit(EXIT_SUCCESS);
		}
	}

	

	// For all the children to be terminated
	int waitRespons, waitStatus = 0;
	while ((waitRespons = wait(&waitStatus)) > 0);

	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		int *data = (int *) (&shMem[i]);
		printf("\n\nshm : %d\n", *data);
	}

	exit(EXIT_SUCCESS);
	
	FILE *pointerFileScore = fopen("Results/score.txt", "w");

	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		fprintf(pointerFileScore, "Car : %d -> Turn : %d\n", arrayCars[i].id, arrayCars[i].turnTimeMS);
	}
	

	fclose(pointerFileScore);
		
	shmdt(shMem);
    shmctl(shmId, IPC_RMID, 0);

	exit(EXIT_SUCCESS);
}