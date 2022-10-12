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
		if(!(fprintf(pointerFileScore, "%d		%d		%d		%d		%d		%s		%s\n", 
		car->id, car->timeSectionMS[0], car->timeSectionMS[1], car->timeSectionMS[2], car->timeTurnMS, (car->state == 2)?"True":"False", (car->state == 1)?"True":"False")))
		{
			perror("fprintf data error ");
			exit(EXIT_FAILURE);
		}
	}

	if(fclose(pointerFileScore))
	{
		perror("fclose error ");
		exit(EXIT_FAILURE);
	}
}

char *PrintBestTimeWithText(int timeMS)
{
}