#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../Headers/Examen.h"
#define LENGTHARRAY(array) (sizeof(array)/sizeof(*array))
#define NUMBEROFCARS 20


// Test for git

struct Car
{
	int id;
	int bestTimeCircuitMS[3];
	int timeCircuitMS[3];
	int turnTimeMS;
	int position; // For later
};

int main()
{
	int upperTimeMaxMS = 45000, lowerTimeMinMS = 25000, arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};

	srand(time(NULL));
	
	struct Car arrayCars[] = {CarBuilder(arrayCarsId)};

	for (int i = 0; i < LENGTHARRAY(arrayCars[0].timeCircuitMS); ++i)
	{
		arrayCars[0].timeCircuitMS[i] = (rand() % (upperTimeMaxMS - lowerTimeMinMS + 1)) + lowerTimeMinMS;
		printf("Temps Random S%d: %d milliseconds (%d seconds)\n", i+1, arrayCars[0].timeCircuitMS[i], MillisecondsToSeconds(arrayCars[0].timeCircuitMS[i]));
		
		if (arrayCars[0].bestTimeCircuitMS[i] == 0 || arrayCars[0].bestTimeCircuitMS[i] > arrayCars[0].timeCircuitMS[i])
		{
			arrayCars[0].bestTimeCircuitMS[i] = arrayCars[0].timeCircuitMS[i];
		}

		arrayCars[0].turnTimeMS += arrayCars[0].timeCircuitMS[i];
	}

	PrintCarTurnTiming(arrayCars[0]);

	return 0;
}

void PrintCarTurnTiming(struct Car car)
{
	int *bestTimeExpr = BestTimeFromMS(car.turnTimeMS);
	printf("Print : %d,      %d\n", bestTimeExpr[0], bestTimeExpr[1]);
	switch(bestTimeExpr[0])
	{
		case 1:
			printf("Car 1 : %d\nTime for the whole turn : %d seconds.\n", car.id, bestTimeExpr[1]);
		break;

		case 2:
			printf("Car 1 : %d\nTime for the whole turn : %d minutes.\n", car.id, bestTimeExpr[1]);
		break;

		case 3:
			printf("Car 1 : %d\nTime for the whole turn : %d hours.\n", car.id, bestTimeExpr[1]);
		break;

		default:
			printf("Car 1 : %d\nTime for the whole turn : %d milliseconds.\n", car.id, car.turnTimeMS);
	}

}

int* BestTimeFromMS(int ms)
{
	int static bestTime[2] = {0, 0};
	if (ms > 2000)
	{
		bestTime[0] = 1;
		bestTime[1] = MillisecondsToSeconds(ms);
		printf("%d\n", bestTime[1]);
	}
	if (bestTime[1] > 120)
	{
		bestTime[0] = 2;
		bestTime[1] = SecondsToMinutes(bestTime[1]);
		printf("%d\n", bestTime[1]);
	}
	if (bestTime[1] > 120)
	{
		bestTime[0] = 3;
		bestTime[1] = MinutesToHours(bestTime[1]);
		printf("%d\n", bestTime[1]);
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

struct Car CarBuilder(int arrayId[])
{
	struct Car arrayCars[NUMBEROFCARS];

	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		arrayCars[i].id = arrayId[i];
		// arrayCars[i].bestTimeCircuitMS;
		arrayCars[i].turnTimeMS = 0;
		// printf("Car built n° %d :\nCar number : %d\n\n", i, arrayCars[i].id);
	}

	return *arrayCars;
}
