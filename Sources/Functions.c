struct Car
{

	int id;
	int bestTimeCircuitMS[3];
	int timeCircuitMS[3];
	int turnTimeMS;
	int position; // For later
	int state; 
	int lastTurnMS;
	int totalTurnMS;
};

void EndOfSession(struct Car *car)
{
	
	car->state = 1;
	printf("Car %d : Out.\n", car->id);
}

void EnterThePits(struct Car *car)
{
	
	car->state = 2;
	printf("Car %d : P.\n", car->id);
}

void PrintBestTimeWithText(int id, int timeMS, char sent[])
{

	int *bestTimeExpr = BestTimeFromMS(timeMS);
	switch(bestTimeExpr[0])
	{

		case 1:
			printf("Car %d : %s : %d seconds.\n", id, sent, bestTimeExpr[1]);
		break;

		case 2:
			printf("Car %d : %s : %d minutes.\n", id, sent, bestTimeExpr[1]);
		break;

		case 3:
			printf("Car %d : %s : %d hours.\n", id, sent, bestTimeExpr[1]);
		break;

		default:
			printf("Car %d : %s : %d milliseconds.\n", id, sent, timeMS);
	}

}

int* BestTimeFromMS(int ms)
{

	int static bestTime[2] = {0, 0};
	if (ms >= 2000)
	{

		bestTime[0] = 1;
		bestTime[1] = MillisecondsToSeconds(ms);
	}
	if (bestTime[1] >= 120)
	{

		bestTime[0] = 2;
		bestTime[1] = SecondsToMinutes(bestTime[1]);
	}
	if (bestTime[1] >= 120)
	{

		bestTime[0] = 3;
		bestTime[1] = MinutesToHours(bestTime[1]);
	}
	
	return bestTime;
}

int MillisecondsToSeconds(int ms)
{

	return ms/1000;
}

int SecondsToMinutes(int secs)
{

	return secs/60;
}

int MinutesToHours(int min)
{

	return	min/60;
}

struct Car *CarBuilder(int arrayId[])
{

	static struct Car arrayCars[NUMBEROFCARS];

	for (int i = 0; i < NUMBEROFCARS; i++)
	{

		arrayCars[i].id = arrayId[i];
		arrayCars[i].turnTimeMS = 0;
		arrayCars[i].position = 0; // For later
		arrayCars[i].lastTurnMS = 0;
		arrayCars[i].totalTurnMS = 0;
		arrayCars[i].state = 0; // Ready to starting
		for (int j = 0; j < LENGTHARRAY(arrayCars[i].timeCircuitMS); j++)
		{
			arrayCars[i].timeCircuitMS[j] = 0;
			arrayCars[i].bestTimeCircuitMS[j] = 0;
		}
		
	}

	return arrayCars;
}

void ResetCar(struct Car *car)
{
	for (int i = 0; i < NUMBEROFCARS; i++)
	{

		car->turnTimeMS = 0;
		car->position = 0; // For later
		car->lastTurnMS = 0;
		car->state = 0; // Ready to starting
		for (int j = 0; j < LENGTHARRAY(car->timeCircuitMS); j++)
		{
			car->timeCircuitMS[j] = 0;
			car->bestTimeCircuitMS[j] = 0;
		}
	}
}

void DoFreeTry(struct Car *car)
{
	
	// While true continue turn testing
	int boolContinueTesting = 1, upperTimeMaxMS = 45000, lowerTimeMinMS = 25000;
	
	ResetCar(car);
	
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
				char sectionNumber[2] = {i+1+'0'};
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
	car->totalTurnMS = 0;
}