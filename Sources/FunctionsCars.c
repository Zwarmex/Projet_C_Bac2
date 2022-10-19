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

Car *CarBuilder(int arrayId[])
{
	static Car arrayCars[NUMBEROFCARS];

	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		arrayCars[i].id = arrayId[i];
		arrayCars[i].totalTurnMS = 0;
		arrayCars[i].boolContinueCompetition = 1;
		InitialisationOrResetCar(&arrayCars[i]);
	}

	return arrayCars;
}

void InitialisationOrResetCar(Car *car)
{
	for (int i = 0; i < NUMBEROFCARS; i++)
	{

		car->timeTurnMS = 0;
		// car->lastTurnMS = 0;
		car->state = 0; // Ready to starting
		car->totalTurnMS = 0;
		for (int j = 0; j < LENGTHARRAY(car->timeSectionMS); j++)
		{
			car->timeSectionMS[j] = 0;
			car->bestTimeSectionMS[j] = 0;
		}
	}
}

void DoRace(Car *car, int minutes, Car *shMem)
{
	int boolContinueTesting = 1, upperTimeMaxMS = 45000, lowerTimeMinMS = 25000;
	
	// Reset the car before the race
	InitialisationOrResetCar(car);
	
	// While true continue turn testing
	while (boolContinueTesting && car->state == 0 && car->totalTurnMS < minutes*60*1000)
	{
		//Reset the time of the turn
		car->timeTurnMS = 0;

		// For each sections
		for (int i = 0; i < LENGTHARRAY(car->timeSectionMS); ++i)
		{
			// Car time is randomize
			car->timeSectionMS[i] = RandomNumber(lowerTimeMinMS, upperTimeMaxMS);
			
			sleep((int)((MSToSeconds(car->timeSectionMS[i])/5)));

			// if the car is doing a better time 
			if (!car->bestTimeSectionMS[i]|| car->bestTimeSectionMS[i] > car->timeSectionMS[i])
			{
				car->bestTimeSectionMS[i] = car->timeSectionMS[i];
			}
			WriteInSharedMemory(shMem, car);
			// kill(getppid(), SIGINT);

			// Keep the time of the circuit
			car->timeTurnMS += car->timeSectionMS[i];
		}
		
		// memcpy(shMem, car, sizeof(*car));
		// kill(getppid(), SIGINT);

		car->totalTurnMS += car->timeTurnMS;

		WriteInSharedMemory(shMem, car);
		// car->lastTurnMS = car->timeTurnMS;

		if (!car->bestTimeTurnMS || car->bestTimeTurnMS > car->timeTurnMS)
		{
			car->bestTimeTurnMS = car->timeTurnMS;
		}

		if(RandomNumber(0, 100) == 1)
		{
			boolContinueTesting = 0;
		}

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
 
// Function to perform Selection Sort
Car *SortArrayCars(Car *arrayCars)
{
	static Car copyArrayCars[NUMBEROFCARS];
	// Copy the array of cars into a new one.
	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		copyArrayCars[i] = arrayCars[i];
	}
	
    // One by one move boundary of unsorted subarray
    for (int i = 0; i < NUMBEROFCARS - 1; i++) 
	{
        // Find the minimum element in unsorted array
        int min_idx = i;
		// while ();  ziehbiezbfezhbfjezhbfjzebfjezbfjezfbjezhfbjezhbfjezhfbezjhfbjezfhb
        for (int j = i + 1; j < NUMBEROFCARS; j++)
		{
            if (copyArrayCars[j].timeTurnMS < copyArrayCars[min_idx].timeTurnMS)
			{
                min_idx = j;
			}
		} 
        // Swap the found minimum element with the first element
		Car temp = copyArrayCars[min_idx];
		copyArrayCars[min_idx] = copyArrayCars[i];
		copyArrayCars[i] = temp;
    }
	return copyArrayCars;
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
    }
	
	memcpy(shMem, car, sizeof(*car));

	if (sem_post(semaParentId) < 0)
	{
		perror("sem_post parent error ");
	}

    if (sem_post(semaChildId) < 0)
    {
        perror("sem_post child error ");
    }
}

void sigint()
{
	if (sem_close(semaParentId) != 0)
	{
		perror("sem_close parent error "); 
		exit(EXIT_FAILURE);
	}

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