typedef struct Car
{
	int id;
	int bestTimeSectionMS[3];
	int timeSectionMS[3];
	int timeTurnMS;
	int position; // For later
	int state; // 1 for Out 2 for pit
	int lastTurnMS;
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
		car->position = 0; // For later
		car->lastTurnMS = 0;
		car->state = 0; // Ready to starting
		car->totalTurnMS = 0;
		for (int j = 0; j < LENGTHARRAY(car->timeSectionMS); j++)
		{
			car->timeSectionMS[j] = 0;
			car->bestTimeSectionMS[j] = 0;
		}
	}
}

void DoFreeTry(Car *car)
{
	int boolContinueTesting = 1, upperTimeMaxMS = 45000, lowerTimeMinMS = 25000;
	
	// Reset the car before the race
	InitialisationOrResetCar(car);
	
	// While true continue turn testing
	while (boolContinueTesting && !car->state)
	{
		//Reset the time of the turn
		car->timeTurnMS = 0;

		// For each sections
		for (int i = 0; i < LENGTHARRAY(car->timeSectionMS); ++i)
		{
			// Car time is randomize
			car->timeSectionMS[i] = rand() % (upperTimeMaxMS - lowerTimeMinMS + 1) + lowerTimeMinMS;

			// printf("go to sleep for : %d\n", car->timeSectionMS[i]);
			// sleep(MSToSeconds(car->timeSectionMS[i]));

			
			// if the car is doing a better time 
			if (car->bestTimeSectionMS[i] == 0 || car->bestTimeSectionMS[i] > car->timeSectionMS[i])
			{
				car->bestTimeSectionMS[i] = car->timeSectionMS[i];
			}

			// Keep the time of the circuit
			car->timeTurnMS += car->timeSectionMS[i];
		}

		car->totalTurnMS += car->timeTurnMS;
		car->lastTurnMS = car->timeTurnMS;

		boolContinueTesting =  rand() % 2;

		if((rand() % 15) == 1)
		{
			EndOfSession(car);
		}

		if((rand() % 15) == 1)
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

void PrintScore(Car *arrayCars)
{
	FILE *pointerFileScore;
	if (!(pointerFileScore = fopen("Results/score.txt", "w")))
	{
		perror("fopen error");
		exit(EXIT_FAILURE);
	}

	// Sort the array of cars
	Car *sortedArrayCars = SortArrayCars(arrayCars);

	if(!(fprintf(pointerFileScore, "Car		S1			S2			S3			TT			PIT			OUT\n")))
	{
		perror("fprintf title error ");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		Car *car = (Car *) (&sortedArrayCars[i]);
		char *arrayBuffersTime[4];
		for (int i = 0; i < 4; i++)
		{
			if(!(arrayBuffersTime[i] = malloc(sizeof(":") * 3 + sizeof(int) * 4)))
			{
				perror("malloc error ");
				exit(EXIT_FAILURE);
			}
		}
		
		if(!(fprintf(pointerFileScore, "%d		%s		%s		%s		%s		%s		%s\n", 
		car->id, returnBestTime(car->timeSectionMS[0], arrayBuffersTime[0]), returnBestTime(car->timeSectionMS[1], arrayBuffersTime[1]), 
		returnBestTime(car->timeSectionMS[2], arrayBuffersTime[2]), returnBestTime(car->timeTurnMS, arrayBuffersTime[3]), 
		(car->state == 2)?"True":"False", (car->state == 1)?"True":"False")))
		{
			perror("fprintf data error ");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < 4; i++)
		{
			free(arrayBuffersTime[i]);
		}
		
	}

	if(fclose(pointerFileScore))
	{
		perror("fclose error ");
		exit(EXIT_FAILURE);
	}
}

char *returnBestTime(int timeMS, char *buff)
{
    int hours = ((timeMS / 1000) / 3600), 
	minutes = ((timeMS / 1000) - (hours * 3600)) / 60, 
	seconds = ((timeMS / 1000) - (hours * 3600) - (minutes * 60)),
	milliseconds = ((timeMS) - (hours * 3600 * 1000) - (minutes * 60 *1000) - (seconds * 1000));

    if (hours)
    {
        char hoursTime[3];
        char minutesTime[3];
        char secondsTime[3];
        char millisecondsTime[4];
        sprintf(hoursTime, "%d", hours);
        sprintf(minutesTime, "%d", minutes);
        sprintf(secondsTime, "%d", seconds);
        sprintf(millisecondsTime, "%d", milliseconds);

        strcpy(buff, hoursTime);
        strcat(buff, ":");
        strcat(buff, minutesTime);
        strcat(buff, ":");
        strcat(buff, secondsTime);
        strcat(buff, ":");
        strcat(buff, millisecondsTime);

    }

    else if (minutes)
    {
        char minutesTime[3];
        char secondsTime[3];
        char millisecondsTime[4];
        sprintf(minutesTime, "%d", minutes);
        sprintf(secondsTime, "%d", seconds);
        sprintf(millisecondsTime, "%d", milliseconds);

        strcpy(buff, minutesTime);
        strcat(buff, ":");
        strcat(buff, secondsTime);
        strcat(buff, ":");
        strcat(buff, millisecondsTime);
    }

    else if (seconds)
    {
        char secondsTime[3];
        char millisecondsTime[4];
        sprintf(secondsTime, "%d", seconds);
        sprintf(millisecondsTime, "%d", milliseconds);

        strcpy(buff, secondsTime);
        strcat(buff, ":");
        strcat(buff, millisecondsTime);
    }

    else
    {
        char millisecondsTime[4];
        sprintf(millisecondsTime, "%d", milliseconds);

        strcpy(buff, millisecondsTime);
    }

    return buff;
}
