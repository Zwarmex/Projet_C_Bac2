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
	int boolSprint = 0, boolClassicWeekEnd = 1, shmSize = sizeof(int) * NUMBEROFCARS, shmData[NUMBEROFCARS],
	shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0775), *shMem = (int *) shmat(shmId, NULL, 0), 
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};

	// Put seed number in rand
	srand(time(NULL));

	// Create array of cars
	Car *arrayCars = CarBuilder(arrayCarsId);

	// Do the race
	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		int pidFork;
		// Fork and check errors
		if ((pidFork= fork()) == -1)
        {
			perror("fork error");
            exit(EXIT_FAILURE);
		}
		
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
				shmData[i] = arrayCars[i].totalTurnMS;
				memmove(shMem + i, (int *) &shmData[i], sizeof(shmData[i]));
			}	
			// Child have to not make another child
			exit(EXIT_SUCCESS);
		}
	}

	int waitRespons, waitStatus = 0;
		
	while ((waitRespons = wait(&waitStatus)) > 0);

	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		printf("\n\nshm : %d\n", shMem + i);
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