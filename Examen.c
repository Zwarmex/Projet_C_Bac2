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
#include <wait.h>
#include "Headers/FunctionsCars.h"
#include "Headers/FunctionsPrinting.h"

int main(int argc, char *argv[]) // Add boolClassicWeekEnd in arg
{
	if (argc < 2)
	{
		printf("No arguments given !\n");
		exit(EXIT_SUCCESS);
	}
	
	// Initialisation of variables
	int shmSize = sizeof(Car) * NUMBEROFCARS,
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};	
	FILE *fp;
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

	// If P1 or P2 or P3
	if ((strcmp(argv[1], "P1") == 0) || (strcmp(argv[1], "P2") == 0) || (strcmp(argv[1], "P3") == 0))
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

			// Child (a car)
			if (pidFork == 0 )
			{
				prctl(PR_SET_PDEATHSIG, SIGHUP);
				signal(SIGHUP, EndOfProgramChild);
				atexit(EndOfProgramChild);
				// Put seed number in rand with pid of the processus
				srand(time(NULL) ^ getpid());
				// Get the sh m from the id

				if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
				{
					perror("shmat error ");
					exit(EXIT_FAILURE);
				}

				DoRace(&arrayCars[i], 2, &shMem[i]);

				// Child have to not make another child
				exit(EXIT_SUCCESS);
			}
		}
		//Parent process
		atexit(EndOfProgramParent);
		signal(SIGINT, EndOfProgramParent);

		// Get the sh m from the id
		if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
		{
			perror("shmat error ");
			exit(EXIT_FAILURE);
		}

		// while minimum a child is alive 
		int waitRespons, waitStatus;
		while ((waitRespons = waitpid(-1, &waitStatus, WNOHANG)) !=-1)
		{
			// break if children are dead
			if ((waitRespons = waitpid(-1, &waitStatus, WNOHANG)) == -1)
			{
				break;
			}

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

		printf("\033c\n");
		FILE *fp;
		if (strcmp(argv[1], "P1") == 0)
		{
			if (!(fp = fopen("ResultSaves/P1.txt", "w")))
			{
				perror("fopen error ");
				exit(EXIT_FAILURE);
			}
		} 
		
		if (strcmp(argv[1], "P2") == 0)
		{
			if (!(fp = fopen("ResultSaves/P2.txt", "w")))
			{
				perror("fopen error ");
				exit(EXIT_FAILURE);
			}
		} 

		if (strcmp(argv[1], "P3") == 0)
		{
			if (!(fp = fopen("ResultSaves/P3.txt", "w")))
			{
				perror("fopen error ");
				exit(EXIT_FAILURE);
			}
		}

		Car *sortedArrayCars = SortArrayCars(shMem);
		for (int i = 0; i < NUMBEROFCARS; i++)
		{
			if((fprintf(fp, "%d\n", sortedArrayCars[i].id)) < 0)
			{
				perror("fprintf error ");
				exit(EXIT_FAILURE);
			}
		}

		if((fclose(fp)) != 0)
		{
			perror("fclose error ");
			exit(EXIT_FAILURE);
		}
		
		printf("Finished\n");
	}
	exit(EXIT_SUCCESS);
}