#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <signal.h>
#include "Headers/FunctionsCars.h"
#include "Headers/FunctionsPrinting.h"


int main() // Add boolClassicWeekEnd in arg
{

	// Initialisation of variables
	int boolSprint = 0, boolClassicWeekEnd = 1, shmSize = sizeof(Car) * NUMBEROFCARS,
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};
	
	Car *arrayCars;

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
			semaChildId = sem_open(semaChildName, O_CREAT, 0600, 1);
			semaParentId = sem_open(semaParentName, O_CREAT, 0600, 0);

			if (semaChildId == SEM_FAILED || semaParentId == SEM_FAILED)
			{
				perror("sem_open error ");
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

			if (sem_close(semaChildId) != 0 || sem_close(semaParentId) != 0)
			{
				perror("sem_close error "); 
				exit(EXIT_FAILURE);
			}

			// if (sem_unlink(semaName) < 0)
			// {
			// 	perror("sem_unlink error "); 
			// 	exit(EXIT_FAILURE);
			// }

			if((shmdt(shMem)) < 0)
			{
				perror("shmdt error ");
				exit(EXIT_FAILURE);
			}
			// Child have to not make another child
			exit(EXIT_SUCCESS);
		}
	}

	// For all the children to be terminated
	// int waitRespons, waitStatus = 0;
	// while(1)
	// {
	// 	// signal(SIGINT, SignalHandler);
	// 	printf("paused");
	// 	pause();
	// }

	signal(SIGINT, sigint);

	semaParentId = sem_open(semaParentName, O_CREAT, 0600, 0);

	if (semaParentId == SEM_FAILED)
	{
		perror("sem_open error ");
		exit(EXIT_FAILURE);
	}

	while(1)
	{
		sem_wait(semaParentId);
		PrintScore(shMem);
		sem_post(semaParentId);
	}
}