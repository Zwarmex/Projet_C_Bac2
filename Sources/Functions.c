typedef struct Car
{
	int id;
	int bestTimeCircuitMS[3];
	int timeCircuitMS[3];
	int turnTimeMS;
	int position; // For later
	int state; 
	int lastTurnMS;
	int totalTurnMS;
}Car;

void EndOfSession(Car *car)
{
	car->state = 1;
	printf("Car %d : Out.\n", car->id);
}

void EnterThePits(Car *car)
{
	car->state = 2;
	printf("Car %d : P.\n", car->id);
}

void PrintBestTimeWithText(int id, int timeMS, char sent[])
{
	int hours = ((timeMS / 1000) / 3600), 
	minutes = ((timeMS / 1000) - (3600 * hours)) / 60, 
	seconds = ((timeMS / 1000) - (3600 * hours) - (minutes * 60));

    if (hours)
    {
        printf("Car %d : %s : %d hours %d minutes %d seconds.\n", id, sent, hours, minutes, seconds);
    }

    else if (minutes)
    {
       	printf("Car %d : %s : %d minutes %d seconds.\n", id, sent, minutes, seconds);
    }

    else if (seconds)
    {
        printf("Car %d : %s : %d seconds.\n", id, sent, seconds);
    }

    else
    {
        printf("Car %d : %s : %d milliseconds.\n", id, sent, timeMS);
    }
}

Car *CarBuilder(int arrayId[])
{
	static Car arrayCars[NUMBEROFCARS];

	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		arrayCars[i].id = arrayId[i];
		arrayCars[i].totalTurnMS = 0;
		InitialisationOrResetCar(&arrayCars[i]);
	}

	return arrayCars;
}

void InitialisationOrResetCar(Car *car)
{
	for (int i = 0; i < NUMBEROFCARS; i++)
	{

		car->turnTimeMS = 0;
		car->position = 0; // For later
		car->lastTurnMS = 0;
		car->state = 0; // Ready to starting
		car->totalTurnMS = 0;
		for (int j = 0; j < LENGTHARRAY(car->timeCircuitMS); j++)
		{
			car->timeCircuitMS[j] = 0;
			car->bestTimeCircuitMS[j] = 0;
		}
	}
}

void DoFreeTry(Car *car)
{
	// While true continue turn testing
	int boolContinueTesting = 1, upperTimeMaxMS = 45000, lowerTimeMinMS = 25000;
	
	InitialisationOrResetCar(car);
	
	while (boolContinueTesting && !car->state)
	{
		car->turnTimeMS = 0;
		// For each sections
		for (int i = 0; i < LENGTHARRAY(car->timeCircuitMS); ++i)
		{
			car->timeCircuitMS[i] = (rand() % (upperTimeMaxMS - lowerTimeMinMS + 1)) + lowerTimeMinMS;
			
			// sleep(MillisecondsToSeconds(car->timeCircuitMS[i]));
			
			// if the car is doing a better time 
			if (car->bestTimeCircuitMS[i] == 0 || car->bestTimeCircuitMS[i] > car->timeCircuitMS[i])
			{
				car->bestTimeCircuitMS[i] = car->timeCircuitMS[i];
				char* sectionAndSentence = malloc(sizeof("S a new best time") + sizeof(int));
				
				strcpy(sectionAndSentence, "S");
				char sectionNumber[2] = {(i+1)+'0'};
				strcat(sectionAndSentence, sectionNumber);
				strcat(sectionAndSentence, " new best time");

				PrintBestTimeWithText(car->id, car->bestTimeCircuitMS[i] , sectionAndSentence);
				
				free(sectionAndSentence);
			}

			// Keep the time of the circuit
			car->turnTimeMS += car->timeCircuitMS[i];
		}

		car->totalTurnMS += car->turnTimeMS;
		
		PrintBestTimeWithText(car->id, car->turnTimeMS, "Turn");

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

	PrintBestTimeWithText(car->id, car->totalTurnMS, "Total");
}