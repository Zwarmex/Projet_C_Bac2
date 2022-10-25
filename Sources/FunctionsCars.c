typedef struct Car
{
	int id;
	int bestTimeSectionMS[3];
	int timeSectionMS[3];
	int timeTurnMS;
	int state; // 1 for Out
	// int lastTurnMS;
	int bestTimeTurnMS;
	int totalTurnMS;
	int boolContinueCompetition;
}Car;

void EndOfSession(Car *car)
{
	car->state = 1;
	// printf("Car %d : Out.\n", car->id);
}

void EnterThePits(Car *car)
{
	car->timeSectionMS[2] += RandomNumber(20000, 28000);
	car->state = 2;
	// printf("Car %d : P.\n", car->id);
}

Car *CarBuilder(int arrayId[], int size)
{
	static Car arrayCarsTMP[NUMBER_OF_CARS];

	for (int i = 0; i < size; i++)
	{
		arrayCarsTMP[i].id = arrayId[i];
		arrayCarsTMP[i].totalTurnMS = 0;
		arrayCarsTMP[i].boolContinueCompetition = 1;
		InitialisationCar(&arrayCarsTMP[i]);
	}

	return arrayCarsTMP;
}

void InitialisationCar(Car *car)
{
	car->timeTurnMS = 0;
	// car->lastTurnMS = 0;
	car->state = 0; // Ready to starting
	car->totalTurnMS = 0;
	for (int j = 0; j < LENGTH_ARRAY(car->timeSectionMS); j++)
	{
		car->timeSectionMS[j] = 0;
		car->bestTimeSectionMS[j] = 0;
	}

}

void DoRace(Car *car, int minutes, Car *shMem)
{
	int boolContinueTesting = 1, upperTimeMaxMS = 45000, lowerTimeMinMS = 25000;
	
	// Reset the car before the race
	InitialisationCar(car);
	
	// While true continue turn testing
	while (boolContinueTesting && car->state == 0 && car->totalTurnMS < minutes*60*1000)
	{
		//Reset the time of the turn
		car->timeTurnMS = 0;

		// For each sections
		for (int i = 0; i < LENGTH_ARRAY(car->timeSectionMS); ++i)
		{
			// Car time is randomize
			car->timeSectionMS[i] = RandomNumber(lowerTimeMinMS, upperTimeMaxMS);
			int seconds = (int)(MSToSeconds(car->timeSectionMS[i])/5);
			if(sleep(seconds) == seconds)
			{
				perror("sleep error ");
				exit(EXIT_FAILURE);
			}

			// if the car is doing a better time 
			if (!car->bestTimeSectionMS[i]|| car->bestTimeSectionMS[i] > car->timeSectionMS[i])
			{
				car->bestTimeSectionMS[i] = car->timeSectionMS[i];
			}
			// kill(getppid(), SIGINT);

			// Keep the time of the circuit
			car->timeTurnMS += car->timeSectionMS[i];
			WriteInSharedMemory(shMem, car);
		}
		
		// memcpy(shMem, car, sizeof(*car));
		// kill(getppid(), SIGINT);

		car->totalTurnMS += car->timeTurnMS;

		// car->lastTurnMS = car->timeTurnMS;

		if (!car->bestTimeTurnMS || car->bestTimeTurnMS > car->timeTurnMS)
		{
			car->bestTimeTurnMS = car->timeTurnMS;
		}
		
		WriteInSharedMemory(shMem, car);

		// if(RandomNumber(0, 100) == 1)
		// {
		// 	boolContinueTesting = 0;
		// }

		if(RandomNumber(0, 500) == 1)
		{
			EndOfSession(car);
		}

		if(RandomNumber(0, 300) == 1)
		{
			EnterThePits(car);
		}
	}
}

float MSToSeconds(int MS)
{
	return MS/1000;
}

int RandomNumber(int min, int max)
{
	return rand() % (max - min + 1) + min;
}

void WriteInSharedMemory(Car *shMem, Car *car)
{    
    if (sem_wait(semaChildId) < 0)
    {
        perror("sem_wait child error ");
		exit(EXIT_FAILURE);
    }
	
	memcpy(shMem, car, sizeof(*car));

	if (sem_post(semaParentId) < 0)
	{
		perror("sem_post parent error ");
		exit(EXIT_FAILURE);
	}

    if (sem_post(semaChildId) < 0)
    {
        perror("sem_post child error ");
		exit(EXIT_FAILURE);
    }
}

void EndOfProgramParent()
{
	shmdt(shMem);
	sem_unlink(semaChildName);
	sem_unlink(semaParentName);
	sem_close(semaChildId);
	sem_close(semaParentId);
	shmctl(shmId, IPC_RMID, 0);
}

void EndOfProgramChild()
{
	shmdt(shMem);
	sem_post(semaChildId);
	sem_post(semaParentId);
	sem_unlink(semaChildName);
	sem_unlink(semaParentName);
	exit(EXIT_SUCCESS);
}