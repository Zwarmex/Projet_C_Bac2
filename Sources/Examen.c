#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "../Headers/Examen.h"
#define LENGTHARRAY(array) (sizeof(array)/sizeof(*array))
#define NUMBEROFCARS 20

struct Car
{

	int id;
	int bestTimeCircuitMS[3];
	int timeCircuitMS[3];
	int turnTimeMS;
	int position; // For later
	int state; 
};

int main()
{

	int upperTimeMaxMS = 45000, lowerTimeMinMS = 25000, arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9},
	boolQualification = 0;

	srand(time(NULL));
	
	struct Car arrayCars[] = {CarBuilder(arrayCarsId)};

	if (!boolQualification)
	{

		int boolContinueTesting = 1;
		while (boolContinueTesting)
		{
			printf("La voiture test le circuit.\n");
			arrayCars[0].turnTimeMS = 0;
			for (int i = 0; i < LENGTHARRAY(arrayCars[0].timeCircuitMS); ++i)
			{

				arrayCars[0].timeCircuitMS[i] = (rand() % (upperTimeMaxMS - lowerTimeMinMS + 1)) + lowerTimeMinMS;
				printf("Temps S%d: %d milliseconds (%d seconds)\n", i+1, arrayCars[0].timeCircuitMS[i], MillisecondsToSeconds(arrayCars[0].timeCircuitMS[i]));
				
				if (arrayCars[0].bestTimeCircuitMS[i] == 0 || arrayCars[0].bestTimeCircuitMS[i] > arrayCars[0].timeCircuitMS[i])
				{

					arrayCars[0].bestTimeCircuitMS[i] = arrayCars[0].timeCircuitMS[i];
				}

				arrayCars[0].turnTimeMS += arrayCars[0].timeCircuitMS[i];
			}
			PrintCarTurnTiming(arrayCars[0]);
			boolContinueTesting =  rand() % 2;
		}
	}

	else
	{
		
	}
	
	
	

	return 0;
}

void PrintCarTurnTiming(struct Car car)
{
	int *bestTimeExpr = BestTimeFromMS(car.turnTimeMS);
	switch(bestTimeExpr[0])
	{

		case 1:
			printf("Car %d : Time for the whole turn : %d seconds.\n", car.id, bestTimeExpr[1]);
		break;

		case 2:
			printf("Car %d : Time for the whole turn : %d minutes.\n", car.id, bestTimeExpr[1]);
		break;

		case 3:
			printf("Car %d : Time for the whole turn : %d hours.\n", car.id, bestTimeExpr[1]);
		break;

		default:
			printf("Car %d : Time for the whole turn : %d milliseconds.\n", car.id, car.turnTimeMS);
	}

}

int* BestTimeFromMS(int ms)
{
	int static bestTime[2] = {0, 0};
	if (ms > 2000)
	{

		bestTime[0] = 1;
		bestTime[1] = MillisecondsToSeconds(ms);
	}
	if (bestTime[1] > 120)
	{

		bestTime[0] = 2;
		bestTime[1] = SecondsToMinutes(bestTime[1]);
	}
	if (bestTime[1] > 120)
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

struct Car CarBuilder(int arrayId[])
{

	struct Car arrayCars[NUMBEROFCARS];

	for (int i = 0; i < NUMBEROFCARS; i++)
	{

		arrayCars[i].id = arrayId[i];
		// arrayCars[i].bestTimeCircuitMS;
		arrayCars[i].turnTimeMS = 0;
		arrayCars[i].state = 0; // Ready to starting
		// printf("Car built n° %d :\nCar number : %d\n\n", i, arrayCars[i].id);
	}

	return *arrayCars;
}
