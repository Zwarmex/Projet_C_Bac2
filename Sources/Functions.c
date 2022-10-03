struct Car
{

	int id;
	int bestTimeCircuitMS[3];
	int timeCircuitMS[3];
	int turnTimeMS;
	int position; // For later
	int state; 
};

struct Car EndOfSession(struct Car car)
{
	
	car.state = 1;
	printf("Car %d : Out.\n", car.id);
	return car;
}

struct Car EnterThePits(struct Car car)
{
	
	car.state = 2;
	printf("Car %d : P.\n", car.id);
	return car;
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
		arrayCars[i].state = 0; // Ready to starting
		for (int j = 0; j < LENGTHARRAY(arrayCars[i].timeCircuitMS); j++)
		{
			arrayCars[i].timeCircuitMS[j] = 0;
			arrayCars[i].bestTimeCircuitMS[j] = 0;
		}
		
	}

	return arrayCars;
}