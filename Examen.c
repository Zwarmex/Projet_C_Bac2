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
	// Initialisation of variables
	int boolSprint = 0, boolClassicWeekEnd = 1, shmSize = sizeof(Car) * NUMBEROFCARS,
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};
	
	Car *arrayCars;

	semaChildId = sem_open("/child", O_CREAT, S_IRUSR | S_IWUSR, 1);
	semaParentId = sem_open("/parent", O_CREAT, S_IRUSR | S_IWUSR, 0);

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

	// For each child
	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		int pidFork;
		// Fork and check errors
		if ((pidFork = fork()) == -1)
        {
			perror("fork error ");
            exit(EXIT_FAILURE);
		}

		atexit(EndOfProgram);

		// signal(SIGINT, sigint);

		// Put seed number in rand with pid of the processus
		srand(time(NULL) ^ getpid());	

		// Get the sh m from the id
		if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
		{
			perror("shmat error ");
			exit(EXIT_FAILURE);
		}

		// Child (a car)
		if (!pidFork)
        {
			if (semaChildId == SEM_FAILED || semaParentId == SEM_FAILED)
			{
				perror("sem_open child error ");
				exit(EXIT_FAILURE);
			}

			if (boolClassicWeekEnd)
			{
				DoRace(&arrayCars[i], 60, &shMem[i]);
				// // memcpy(&shMem[i], &arrayCars[i], sizeof(arrayCars[i]));
				// DoRace(&arrayCars[i], 60, &shMem[i], semaphore);
				// // memcpy(&shMem[i], &arrayCars[i], sizeof(arrayCars[i]));
				// DoRace(&arrayCars[i], 60, &shMem[i], semaphore);

				// DoRace(&arrayCars[i], 18, &shMem[i], semaphore);

				// memcpy(&shMem[i], &arrayCars[i], sizeof(arrayCars[i]));
			}

			// Child have to not make another child
			exit(EXIT_SUCCESS);
		}
	}

	signal(SIGINT, EndOfProgram);

	if (semaParentId == SEM_FAILED)
	{
		perror("sem_open parent error ");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		if(sem_wait(semaParentId) < 0)
		{
			perror("sem_wait parent error ");
			exit(EXIT_FAILURE);
		}

		PrintScore(shMem);
	}
}