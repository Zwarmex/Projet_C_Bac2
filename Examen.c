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
#include "Headers/FunctionsCars.h"
#include "Headers/FunctionsPrinting.h"

int main() // Add boolClassicWeekEnd in arg
{

	// Initialisation of variables
	int boolSprint = 0, boolClassicWeekEnd = 1, shmSize = sizeof(Car) * NUMBEROFCARS, shmId,
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};

	Car *shMem;


	// Get id for the sh m
	if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0775)) < 0)
	{
		perror("shmget error ");
		exit(EXIT_FAILURE);
	} 

	// Create array of cars
	Car *arrayCars;
	if (!(arrayCars = CarBuilder(arrayCarsId)))
	{
		printf("CarBuilder error");
		exit(EXIT_FAILURE);
	}

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
		srand(time(NULL) ^ getpid());	

		// Get the sh m from the id
		if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
		{
			perror("shmat error ");
			exit(EXIT_FAILURE);
		}

		// Child (a car)
		if (pidFork == 0)
        {	
			if (boolClassicWeekEnd)
			{
				DoRace(&arrayCars[i], 60, &shMem[i]);
				// memcpy(&shMem[i], &arrayCars[i], sizeof(arrayCars[i]));
				DoRace(&arrayCars[i], 60, &shMem[i]);
				// memcpy(&shMem[i], &arrayCars[i], sizeof(arrayCars[i]));
				DoRace(&arrayCars[i], 60, &shMem[i]);

				DoRace(&arrayCars[i], 18, &shMem[i]);

				// memcpy(&shMem[i], &arrayCars[i], sizeof(arrayCars[i]));


				if((shmdt(shMem)) < 0)
				{
					perror("shmdt error ");
					exit(EXIT_FAILURE);
				}
				
			}	
			// Child have to not make another child
			exit(EXIT_SUCCESS);
		}
	}

	// For all the children to be terminated
	int waitRespons, waitStatus = 0;
	while ((waitRespons = wait(&waitStatus)) > 0);

	PrintScore(shMem);

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