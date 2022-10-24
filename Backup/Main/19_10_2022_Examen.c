#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <signal.h>
#include <sys/stat.h>
#include "Headers/FunctionsCars.h"
#include "Headers/FunctionsPrinting.h"


int main() // Add boolClassicWeekEnd in arg
{
	atexit(EndOfProgram);
	signal(SIGINT, EndOfProgram);

	// Initialisation of variables
	int boolSprint = 0, boolClassicWeekEnd = 1, shmSize = sizeof(Car) * NUMBEROFCARS,
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};
	
	Car *arrayCars;

	semaChildId = sem_open(semaChildName, O_CREAT, S_IRUSR | S_IWUSR, 1);
	semaParentId = sem_open(semaParentName, O_CREAT, S_IRUSR | S_IWUSR, 0);

	if (semaChildId == SEM_FAILED || semaParentId == SEM_FAILED)
	{
		perror("sem_open child error ");
		exit(EXIT_FAILURE);
	}

	// Get id for the sh m
	if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0775)) < 0)
	{
		perror("shmget error ");
		exit(EXIT_FAILURE);
	}

	// Create array of cars
	if (!(arrayCars = CarBuilder(arrayCarsId)))
	{
		printf("CarBuilder error");
		exit(EXIT_FAILURE);
	}

	// Classic Weekend
	if (boolClassicWeekEnd == 1)
    {
		// 3 Free try 
		for (int k = 0;  k < 3; k++)
		{
			// For each child
			for (int i = 0; i < NUMBEROFCARS; i++)
			{
				int pidFork = fork();
				// Fork and check errors
				if (pidFork == -1)
				{
					perror("fork error ");
					exit(EXIT_FAILURE);
				}

				// Get the sh m from the id
				if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
				{
					perror("shmat error ");
					exit(EXIT_FAILURE);
				}

				// Child (a car)
				if (!pidFork)
				{
					// Put seed number in rand with pid of the processus
					srand(time(NULL) ^ getpid());

					DoRace(&arrayCars[i], 60, &shMem[i]);

					// Child have to not make another child
					exit(EXIT_SUCCESS);
				}
			}
			int statusWait;
			while (wait(&statusWait))
			{
				WhileChildrenAreBusy();
			}			
		}
		
		// Q1
		// for (int i = 0; i < NUMBEROFCARS; i++)
		// {
		// 	int pidFork;
		// 	// Fork and check errors
		// 	if ((pidFork = fork()) == -1)
		// 	{
		// 		perror("fork error ");
		// 		exit(EXIT_FAILURE);
		// 	}
		// 	atexit(EndOfProgram);
		// 	// Put seed number in rand with pid of the processus
		// 	srand(time(NULL) ^ getpid());	
		// 	// Get the sh m from the id
		// 	if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
		// 	{
		// 		perror("shmat error ");
		// 		exit(EXIT_FAILURE);
		// 	}
		// 	// Child (a car)
		// 	if (!pidFork)
		// 	{
		// 		DoRace(&arrayCars[i], 60, &shMem[i]);
		// 		// Child have to not make another child
		// 		exit(EXIT_SUCCESS);
		// 	}
		// }		
		// Car *sortedArrayCars = SortArrayCars(shMem);
		// printf("week end over\n");
	}

	
}