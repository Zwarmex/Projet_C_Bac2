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
#include <sys/prctl.h>
#include "Headers/FunctionsCars.h"
#include "Headers/FunctionsPrinting.h"

int main(int argc, char *argv[]) // Add boolClassicWeekEnd in arg
{
	if (argc < 2)
	{
		printf("No arguments given !\n");
		exit(EXIT_SUCCESS);
	}
	
	atexit(EndOfProgram);
	signal(SIGINT, EndOfProgram);

	// Initialisation of variables
	int shmSize = sizeof(Car) * NUMBEROFCARS,
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};
	
	Car *arrayCars;

	semaChildId = sem_open(semaChildName, O_CREAT, S_IRUSR | S_IWUSR, 1);
	semaParentId = sem_open(semaParentName, O_CREAT, S_IRUSR | S_IWUSR, 0);

	if (semaChildId == SEM_FAILED || semaParentId == SEM_FAILED)
	{
		perror("sem_open error ");
		exit(EXIT_FAILURE);
	}

	// Get id for the sh m
	if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0666)) < 0)
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

	// If P1
	if (strcmp(argv[1], "P1") == 0)
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
			if (pidFork == 0 )
			{
				prctl(PR_SET_PDEATHSIG, SIGHUP);
				signal(SIGHUP, EndOfProgram);
				atexit(EndOfProgram);
				// Put seed number in rand with pid of the processus
				srand(time(NULL) ^ getpid());

				DoRace(&arrayCars[i], 60, &shMem[i]);

				// Child have to not make another child
				exit(EXIT_SUCCESS);
			}
		}
		while (1)
		{
			// Wait that a child wrote in shm
			if(sem_wait(semaParentId) < 0)
			{
				perror("sem_wait parent error ");
				exit(EXIT_FAILURE);
			}

			// Wait for read the shm
			if (sem_wait(semaChildId) < 0)
			{
				perror("sem_wait parent error ");
				exit(EXIT_FAILURE);
			}

			int value;

			if (sem_getvalue(semaParentId, &value) < 0)
			{
				perror("sem_getvalue parent error ");
				exit(EXIT_FAILURE);
			}

			// Decrementing sema parent 
			while (value > 0)
			{
				if(sem_wait(semaParentId) < 0)
				{
					perror("sem_wait parent error ");
					exit(EXIT_FAILURE);
				}
				if (sem_getvalue(semaParentId, &value) < 0)
				{
					perror("sem_getvalue parent error ");
					exit(EXIT_FAILURE);
				}
			}
			// Critical section

			PrintScore(shMem);

			// printf("parent section critical\n");

			// Unlock the shm
			if (sem_post(semaChildId) < 0)
			{
				perror("sem_post parent error ");
				exit(EXIT_FAILURE);
			}
		}
	}
	
	exit(EXIT_SUCCESS);
}