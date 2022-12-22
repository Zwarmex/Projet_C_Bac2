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
#include <limits.h>
#include "Headers/FunctionsCars.h"
#include "Headers/FunctionsPrinting.h"

int main(int argc, char *argv[]) // Add boolClassicWeekEnd in arg
{
	if (argc < 2)
	{
		printf("No race section given !\n");
		exit(EXIT_FAILURE);
	}

	if (argc < 3)
	{
		printf("No minutes max given !\n");
		exit(EXIT_FAILURE);
	}
	
	if (argc < 4)
	{
		printf("No number of turns max given !\n");
		exit(EXIT_FAILURE);
	}

	char *endptr;
	int minutesOfRace = strtol(argv[2], &endptr, 10);
	if (endptr != "") 
	{
		printf("No valid minutes max given !\n");
		exit(EXIT_FAILURE);
	}
	int numberOfTurnMax = strtol(argv[3], &endptr, 10);
	if (endptr == argv[3]) 
	{
		printf("No valid number of turns max given !\n");
		exit(EXIT_FAILURE);
	}
	Car *arrayCars;
	semaChildId = sem_open(semaChildName, O_CREAT, S_IRUSR | S_IWUSR, 1);
	semaParentId = sem_open(semaParentName, O_CREAT, S_IRUSR | S_IWUSR, 0);
	
	// Initialisation of variables


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
			perror("shm get error ");
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

				// Put seed number in rand with pid of the processes
				srand(time(NULL) ^ getpid());
				
				// Get the sh m from the id
				if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
				{
					perror("shm-at error ");
					exit(EXIT_FAILURE);
				}

				// Create array of cars
				if (!(arrayCars = CarBuilder(arrayCarsId, NUMBER_OF_CARS)))
				{
					printf("CarBuilder error");
					exit(EXIT_FAILURE);
				}

				DoRace(&arrayCars[i], minutesOfRace, &shMem[i], numberOfTurnMax, 0);

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
			perror("shm get error ");
			exit(EXIT_FAILURE);
		}

		int arrayCarsId[NUMBER_OF_CARS], counter = 0;	
		FILE *PFile = fopen("ResultSaves/P3.txt", "r");
		if (!PFile)
		{
			perror("open error ");
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
				
				DoRace(&arrayCars[i], minutesOfRace, &shMem[i], numberOfTurnMax, 0);

				// Child have to not make another child
				exit(EXIT_SUCCESS);
			}
		}
	}
// test
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
			perror("close error ");
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

				// Put seed number in rand with pid of the processes
				srand(time(NULL) ^ getpid());
				
				// Get the sh m from the id
				if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
				{
					perror("shm-at error ");
					exit(EXIT_FAILURE);
				}
				
				DoRace(&arrayCars[i], minutesOfRace, &shMem[i],numberOfTurnMax, 0);

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
			perror("shm get error ");
			exit(EXIT_FAILURE);
		}

		int arrayCarsId[NUMBER_OF_CARS_Q3], counter = 0;	
		FILE *PFile = fopen("ResultSaves/Q2.txt", "r");
		if (!PFile)
		{
			perror("open error ");
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

				// Put seed number in rand with pid of the processes
				srand(time(NULL) ^ getpid());
				
				// Get the sh m from the id
				if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
				{
					perror("shm-at error ");
					exit(EXIT_FAILURE);
				}
				
				DoRace(&arrayCars[i], minutesOfRace, &shMem[i],numberOfTurnMax, 0);

				// Child have to not make another child
				exit(EXIT_SUCCESS);
			}
		}
	}
	
	// If Race
	else if (strcmp(argv[1], "Race") == 0)
	{
		int shmSize = sizeof(Car) * NUMBER_OF_CARS;
		// Get id for the sh m
		if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0666)) < 0)
		{
			perror("shm get error ");
			exit(EXIT_FAILURE);
		}

		int arrayFirstCarsId[NUMBER_OF_CARS], counter = 0;	
		FILE *PFile = fopen("ResultSaves/Q3.txt", "r");
		if (!PFile)
		{
			perror("open error ");
			exit(EXIT_FAILURE);
		}
		
		char currentline[3];

		while ((fgets(currentline, sizeof(currentline), PFile) != NULL) && (counter < NUMBER_OF_CARS_Q3)) 
		{
			if (*currentline != '\n')
			{					
				// printf("got line: %s\n", currentline);
				arrayFirstCarsId[counter] = atoi(currentline);
				counter++;
			}
		}

		int number_of_car_found = 0;

		for (int i = 0; i < NUMBER_OF_CARS_Q3; i++)
		{
			int found = 0;
			for (int j = 0; j < NUMBER_OF_CARS; j++)
			{
				if (arrayCarsId[i] == arrayFirstCarsId[j])
				{
					found = 1;
					break;
				}
			}
			if (!found)
			{
				arrayFirstCarsId[(int)(NUMBER_OF_CARS/2) + number_of_car_found] = arrayCarsId[i];
			}
		}
		
		
		
		if(fclose(PFile) != 0)
		{
			perror("close error ");
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

				// Put seed number in rand with pid of the processes
				srand(time(NULL) ^ getpid());
				
				// Get the sh m from the id
				if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
				{
					perror("shm-at error ");
					exit(EXIT_FAILURE);
				}
				
				DoRace(&arrayCars[i], minutesOfRace, &shMem[i], numberOfTurnMax, 1);

				// Child have to not make another child
				exit(EXIT_SUCCESS);
			}
		}
	}
	
	else if (strcmp(argv[1], "Sprint") == 0)
	{
		int shmSize = sizeof(Car) * NUMBER_OF_CARS;
		// Get id for the sh m
		if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0666)) < 0)
		{
			perror("shm get error ");
			exit(EXIT_FAILURE);
		}

		int arrayCarsId[NUMBER_OF_CARS_Q3], counter = 0;	
		FILE *PFile = fopen("ResultSaves/Q3.txt", "r");
		if (!PFile)
		{
			perror("open error ");
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
			perror("close error ");
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

				// Put seed number in rand with pid of the processes
				srand(time(NULL) ^ getpid());
				
				// Get the sh m from the id
				if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
				{
					perror("shm-at error ");
					exit(EXIT_FAILURE);
				}
				
				DoRace(&arrayCars[i], minutesOfRace, &shMem[i], numberOfTurnMax, 1);

				// Child have to not make another child
				exit(EXIT_SUCCESS);
			}
		}
	}

	else
	{
		printf("No race chosen\n");
		exit(EXIT_SUCCESS);
	}


	//Parent process
	atexit(EndOfProgramParent);
	signal(SIGINT, EndOfProgramParent);

	// Get the sh m from the id
	if ((shMem = (Car *) shmat(shmId, NULL, 0)) < 0)
	{
		perror("shm-at error ");
		exit(EXIT_FAILURE);
	}

	int waitRespons, waitStatus;
	struct timespec ts;
	ts.tv_sec = 0;
    ts.tv_nsec = 100000000; // 100 ms
	
	// while minimum a child is alive 
	while ((waitRespons = waitpid(-1, &waitStatus, WNOHANG)) !=-1)
	{
		// Add a timout ont the semaphore 
		if (sem_timedwait(semaParentId, &ts) == -1)
		{	
			// Check if children are not dead
			if ((waitRespons = waitpid(-1, &waitStatus, WNOHANG)) == -1)
			{
				break;
			}			
		}
		else
		{
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
			
			else if (strcmp(argv[1], "Q3") == 0 || strcmp(argv[1], "Race") == 0)
			{
				PrintScore(shMem, NUMBER_OF_CARS_Q3);
			}

			// Unlock the shm
			if (sem_post(semaChildId) < 0)
			{
				perror("sem_post parent error ");
				exit(EXIT_FAILURE);
			}
		}
	}
		
	FILE *fp;

	if (strcmp(argv[1], "P1") == 0)
	{
		if (!(fp = fopen("ResultSaves/P1.txt", "w")))
		{
			perror("open error in : P1.txt ");
			exit(EXIT_FAILURE);
		}
	} 
	
	else if (strcmp(argv[1], "P2") == 0)
	{
		if (!(fp = fopen("ResultSaves/P2.txt", "w")))
		{
			perror("open error in : P2.txt ");
			exit(EXIT_FAILURE);
		}
	} 

	else if (strcmp(argv[1], "P3") == 0)
	{
		if (!(fp = fopen("ResultSaves/P3.txt", "w")))
		{
			perror("open error in : P3.txt ");
			exit(EXIT_FAILURE);
		}
	}
	
	else if (strcmp(argv[1], "Q1") == 0)
	{
		if (!(fp = fopen("ResultSaves/Q1.txt", "w")))
		{
			perror("open error in : Q1.txt ");
			exit(EXIT_FAILURE);
		}
	}
	
	else if (strcmp(argv[1], "Q2") == 0)
	{
		if (!(fp = fopen("ResultSaves/Q2.txt", "w")))
		{
			perror("open error in : Q2.txt ");
			exit(EXIT_FAILURE);
		}
	}
	
	else if (strcmp(argv[1], "Q3") == 0)
	{
		if (!(fp = fopen("ResultSaves/Q3.txt", "w")))
		{
			perror("open error in : Q3.txt");
			exit(EXIT_FAILURE);
		}
	}
	
	else if (strcmp(argv[1], "Race") == 0)
	{
		if (!(fp = fopen("ResultSaves/Race.txt", "w")))
		{
			perror("open error in : Race.txt");
			exit(EXIT_FAILURE);
		}
	}

	else if (strcmp(argv[1], "Sprint") == 0)
	{
		if (!(fp = fopen("ResultSaves/Sprint.txt", "w")))
		{
			perror("open error in : Sprint.txt");
			exit(EXIT_FAILURE);
		}
	}

	else
	{
		print("No file found\n");
		exit(EXIT_FAILURE);
	}
	
	Car *sortedArrayCars = SortArrayCars(shMem);
	for (int i = 0; i < NUMBER_OF_CARS; i++)
	{
		if (sortedArrayCars[i].id)
		{
			if((fprintf(fp, "%d\n", sortedArrayCars[i].id)) < 0)
			{
				perror("f_print_f error ");
				exit(EXIT_FAILURE);
			}
		}
	}

    char *arrayBuffersBestTime[BEST_TIME_BUFFER];
    int bestS1 = INT_MAX, bestS2 = INT_MAX, bestS3 = INT_MAX;
    for (int i = 0; i < NUMBER_OF_CARS; ++i)
    {
        Car *car = (Car *) (&sortedArrayCars[i]);
        if (bestS1 > car->bestTimeSectionMS[0])
        {
            bestS1 = car->bestTimeSectionMS[0];
        }
        if (bestS2 > car->bestTimeSectionMS[1])
        {
            bestS2 = car->bestTimeSectionMS[1];
        }
        if (bestS3 > car->bestTimeSectionMS[2])
        {
            bestS3 = car->bestTimeSectionMS[2];
        }
    }
    for (int i = 0; i < BEST_TIME_BUFFER; i++)
    {
        if(!(arrayBuffersBestTime[i] = malloc(sizeof(":") * 3 + sizeof(int) * 4)))
        {
            perror("malloc error ");
            exit(EXIT_FAILURE);
        }
    }
    if((fprintf(fp, "Best TT : %s\nBest S1 : %s\nBest S2 : %s\nBest S2 : %s\n",
           returnBestTime(sortedArrayCars[0].bestTimeTurnMS, arrayBuffersBestTime[0]),
           returnBestTime(bestS1, arrayBuffersBestTime[1]),
           returnBestTime(bestS2, arrayBuffersBestTime[2]),
           returnBestTime(bestS3, arrayBuffersBestTime[3]))) < 0)
			{
				perror("f_print_f error ");
				exit(EXIT_FAILURE);
			}

	if((fclose(fp)) != 0)
	{
		perror("close error ");
		exit(EXIT_FAILURE);
	}
	
	printf("\033cRace finished\n");
	exit(EXIT_SUCCESS);
}