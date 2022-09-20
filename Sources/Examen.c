#include <stdio.h>
#include <stdlib.h>
#include "../Headers/Examen.h"
#define LENGTHARRAY(x) (sizeof(x)/sizeof(*x))


struct Car
{
	int idCar;
	int bestTimeCircuitMS[3];
	int timeCircuitMS[3];
	int position; // For later
};

int main()
{
	int upperTimeMaxMS = 45000, lowerTimeMinMS = 25000, bestTimeCircuitMS[3] = {0, 0, 0}, timeCircuitMS[3], grandPrice, boolClassicWeekend = 1, arrayCarsId[20] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};
	
	struct Car arrayCars[20] = CarBuilder(*arrayCarsId);

	for (int i = 0; i < LENGTHARRAY(timeCircuitMS); ++i)
	{
		timeCircuitMS[i] = (rand() % (upperTimeMaxMS - lowerTimeMinMS + 1)) + lowerTimeMinMS;
		printf("Temps Random S%d: %d milliseconds (%d seconds)\n", i+1, timeCircuitMS[i], MillisecondsToSeconds(timeCircuitMS[i]));
		
		if (bestTimeCircuitMS[i] == 0 || bestTimeCircuitMS[i] > timeCircuitMS[i])
		{
			bestTimeCircuitMS[i] = timeCircuitMS[i];
		}
	}

	int bestTime = timeCircuitMS[0];

	for (int i = 1; i < LENGTHARRAY(timeCircuitMS); ++i)
	{
		if (bestTime > timeCircuitMS[i])
		{
			bestTime = timeCircuitMS[i];
		}
	}


	printf("Best time : %d milliseconds (%d seconds)\n", bestTime, MillisecondsToSeconds(bestTime));
	printf("Car 1 : %d\n", arrayCars[0].idCar);
	return 0;
}

int MillisecondsToSeconds(int ms)
{
	return ms/1000;
}

int SecondsToMinutes(int secs)
{
	return secs/60;
}

struct Car CarBuilder(int arrayIdCars)
{
	for (int i = 0; i < LENGTHARRAY(arrayIdCars); ++i)
	{
		arrayCars[i].idCar = arrayCarsId[i];
		printf("Car built n° %d\n", i);
	}

	return arrayCars
}
