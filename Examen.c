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
	int minutesOfRace = atoi(argv[2]);
	Car *arrayCars;
	semaChildId = sem_open(semaChildName, O_CREAT, S_IRUSR | S_IWUSR, 1);
	semaParentId = sem_open(semaParentName, O_CREAT, S_IRUSR | S_IWUSR, 0);

	if (semaChildId == SEM_FAILED || semaParentId == SEM_FAILED)
	{
		perror("sem_open error ");
		exit(EXIT_FAILURE);
	}

	// If P1 or P2 or P3
	if ((strcmp(argv[1], "P1") == 0) || (strcmp(argv[1], "P2") == 0) || (strcmp(argv[1], "P3") == 0))
    {
		int shmSize = sizeof(Car) * NUMBER_OF_CARS;
		// Get id for the sh m
		if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0666)) < 0)
		{
			perror("shmget error ");
			exit(EXIT_FAILURE);
		}

		// For each child
		for (int i = 0; i < NUMBER_OF_CARS; i++)
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

				int arrayCarsId[NUMBER_OF_CARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};	
				
				// Create array of cars
				if (!(arrayCars = CarBuilder(arrayCarsId, NUMBER_OF_CARS)))
				{
					printf("CarBuilder error");
					exit(EXIT_FAILURE);
				}

				DoRace(&arrayCars[i], minutesOfRace, &shMem[i]);

				// Child have to not make another child
				exit(EXIT_SUCCESS);
			}
		}
	}
	
	// If Q1
	else if (strcmp(argv[1], "Q1") == 0)
	{
		int shmSize = sizeof(Car) * NUMBER_OF_CARS;
		// Get id for the sh m
		if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0666)) < 0)
		{
			perror("shmget error ");
			exit(EXIT_FAILURE);
		}

		int arrayCarsId[NUMBER_OF_CARS], counter = 0;	
		FILE *PFile = fopen("ResultSaves/P3.txt", "r");
		if (!PFile)
		{
			perror("fopen error ");
			exit(EXIT_FAILURE);
		}
		
		char currentline[3];

		while (fgets(currentline, sizeof(currentline), PFile) != NULL) 
		{
			if (*currentline != '\n')
			{					
				// printf("got line: %s\n", currentline);
				arrayCarsId[counter] = atoi(currentline);
				counter++;
			}
		}
		if(fclose(PFile) != 0)
		{
			perror("fclose error ");
			exit(EXIT_FAILURE);
		}

		// Create array of cars
		if (!(arrayCars = CarBuilder(arrayCarsId, NUMBER_OF_CARS)))
		{
			printf("CarBuilder error");
			exit(EXIT_FAILURE);
		}
		
		// For each child
		for (int i = 0; i < NUMBER_OF_CARS; i++)
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
				
				DoRace(&arrayCars[i], minutesOfRace, &shMem[i]);

				// Child have to not make another child
				exit(EXIT_SUCCESS);
			}
		}
	}

	// If Q2
	else if (strcmp(argv[1], "Q2") == 0)
	{
		int shmSize = sizeof(Car) * NUMBER_OF_CARS_Q2;
		// Get id for the sh m
		if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0666)) < 0)
		{
			perror("shmget error ");
			exit(EXIT_FAILURE);
		}

		int arrayCarsId[NUMBER_OF_CARS_Q2], counter = 0;	
		FILE *PFile = fopen("ResultSaves/Q1.txt", "r");
		if (!PFile)
		{
			perror("fopen error ");
			exit(EXIT_FAILURE);
		}
		
		char currentline[3];

		while ((fgets(currentline, sizeof(currentline), PFile) != NULL) && (counter < NUMBER_OF_CARS_Q2)) 
		{
			if (*currentline != '\n')
			{					
				// printf("got line: %s\n", currentline);
				arrayCarsId[counter] = atoi(currentline);
				counter++;
			}
		}
		
		if(fclose(PFile) != 0)
		{
			perror("fclose error ");
			exit(EXIT_FAILURE);
		}

		// Create array of cars
		if (!(arrayCars = CarBuilder(arrayCarsId, NUMBER_OF_CARS_Q2)))
		{
			printf("CarBuilder error");
			exit(EXIT_FAILURE);
		}
		
		// For each child
		for (int i = 0; i < NUMBER_OF_CARS_Q2; i++)
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
				
				DoRace(&arrayCars[i], minutesOfRace, &shMem[i]);

				// Child have to not make another child
				exit(EXIT_SUCCESS);
			}
		}
	}

	// If Q3
	else if (strcmp(argv[1], "Q3") == 0)
	{
		int shmSize = sizeof(Car) * NUMBER_OF_CARS_Q3;
		// Get id for the sh m
		if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0666)) < 0)
		{
			perror("shmget error ");
			exit(EXIT_FAILURE);
		}

		int arrayCarsId[NUMBER_OF_CARS_Q3], counter = 0;	
		FILE *PFile = fopen("ResultSaves/Q2.txt", "r");
		if (!PFile)
		{
			perror("fopen error ");
			exit(EXIT_FAILURE);
		}
		
		char currentline[3];

		while ((fgets(currentline, sizeof(currentline), PFile) != NULL) && (counter < NUMBER_OF_CARS_Q3)) 
		{
			if (*currentline != '\n')
			{					
				// printf("got line: %s\n", currentline);
				arrayCarsId[counter] = atoi(currentline);
				counter++;
			}
		}
		
		if(fclose(PFile) != 0)
		{
			perror("fclose error ");
			exit(EXIT_FAILURE);
		}

		// Create array of cars
		if (!(arrayCars = CarBuilder(arrayCarsId, NUMBER_OF_CARS_Q3)))
		{
			printf("CarBuilder error");
			exit(EXIT_FAILURE);
		}
		
		// For each child
		for (int i = 0; i < NUMBER_OF_CARS_Q3; i++)
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
				
				DoRace(&arrayCars[i], minutesOfRace, &shMem[i]);

				// Child have to not make another child
				exit(EXIT_SUCCESS);
			}
		}
	}
	
	// If Race
	else if (strcmp(argv[1], "Race") == 0)
	{
		int shmSize = sizeof(Car) * NUMBER_OF_CARS_Q3;
		// Get id for the sh m
		if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0666)) < 0)
		{
			perror("shmget error ");
			exit(EXIT_FAILURE);
		}

		int arrayCarsId[NUMBER_OF_CARS_Q3], counter = 0;	
		FILE *PFile = fopen("ResultSaves/Q3.txt", "r");
		if (!PFile)
		{
			perror("fopen error ");
			exit(EXIT_FAILURE);
		}
		
		char currentline[3];

		while ((fgets(currentline, sizeof(currentline), PFile) != NULL) && (counter < NUMBER_OF_CARS_Q3)) 
		{
			if (*currentline != '\n')
			{					
				// printf("got line: %s\n", currentline);
				arrayCarsId[counter] = atoi(currentline);
				counter++;
			}
		}
		
		if(fclose(PFile) != 0)
		{
			perror("fclose error ");
			exit(EXIT_FAILURE);
		}

		// Create array of cars
		if (!(arrayCars = CarBuilder(arrayCarsId, NUMBER_OF_CARS_Q3)))
		{
			printf("CarBuilder error");
			exit(EXIT_FAILURE);
		}
		
		// For each child
		for (int i = 0; i < NUMBER_OF_CARS_Q3; i++)
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
				
				DoRace(&arrayCars[i], minutesOfRace, &shMem[i]);

				// Child have to not make another child
				exit(EXIT_SUCCESS);
			}
		}
	}
	else
	{
		printf("No race choosen\n");
		exit(EXIT_SUCCESS);
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

	int waitRespons, waitStatus;
	// while minimum a child is alive 
	while ((waitRespons = waitpid(-1, &waitStatus, WNOHANG)) !=-1)
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

		if (strcmp(argv[1], "P1") == 0 || strcmp(argv[1], "P2") == 0 || strcmp(argv[1], "P3") == 0 || strcmp(argv[1], "Q1") == 0)
		{
			PrintScore(shMem, NUMBER_OF_CARS);
		}
		
		else if (strcmp(argv[1], "Q2") == 0)
		{
			PrintScore(shMem, NUMBER_OF_CARS_Q2);
		}
		
		else if (strcmp(argv[1], "Q3") == 0)
		{
			PrintScore(shMem, NUMBER_OF_CARS_Q3);
		}

		// Unlock the shm
		if (sem_post(semaChildId) < 0)
		{
			perror("sem_post parent error ");
			exit(EXIT_FAILURE);
		}
		
		// break if children are dead
		if ((waitRespons = waitpid(-1, &waitStatus, WNOHANG)) == -1)
		{
			break;
		}
	}
		
	FILE *fp;

	if (strcmp(argv[1], "P1") == 0)
	{
		if (!(fp = fopen("ResultSaves/P1.txt", "w")))
		{
			perror("fopen error ");
			exit(EXIT_FAILURE);
		}
	} 
	
	else if (strcmp(argv[1], "P2") == 0)
	{
		if (!(fp = fopen("ResultSaves/P2.txt", "w")))
		{
			perror("fopen error ");
			exit(EXIT_FAILURE);
		}
	} 

	else if (strcmp(argv[1], "P3") == 0)
	{
		if (!(fp = fopen("ResultSaves/P3.txt", "w")))
		{
			perror("fopen error ");
			exit(EXIT_FAILURE);
		}
	}
	
	else if (strcmp(argv[1], "Q1") == 0)
	{
		if (!(fp = fopen("ResultSaves/Q1.txt", "w")))
		{
			perror("fopen error ");
			exit(EXIT_FAILURE);
		}
	}
	
	else if (strcmp(argv[1], "Q2") == 0)
	{
		if (!(fp = fopen("ResultSaves/Q2.txt", "w")))
		{
			perror("fopen error ");
			exit(EXIT_FAILURE);
		}
	}
	
	else if (strcmp(argv[1], "Q3") == 0)
	{
		if (!(fp = fopen("ResultSaves/Q3.txt", "w")))
		{
			perror("fopen error ");
			exit(EXIT_FAILURE);
		}
	}
	
	else if (strcmp(argv[1], "Race") == 0)
	{
		if (!(fp = fopen("ResultSaves/Race.txt", "w")))
		{
			perror("fopen error ");
			exit(EXIT_FAILURE);
		}
	}
	
	Car *sortedArrayCars = SortArrayCars(shMem);
	for (int i = 0; i < NUMBER_OF_CARS; i++)
	{
		if (sortedArrayCars[i].id)
		{
			if((fprintf(fp, "%d\n", sortedArrayCars[i].id)) < 0)
			{
				perror("fprintf error ");
				exit(EXIT_FAILURE);
			}
		}
	}

	if((fclose(fp)) != 0)
	{
		perror("fclose error ");
		exit(EXIT_FAILURE);
	}
	
	printf("\033cRace finished\n");
	exit(EXIT_SUCCESS);
}