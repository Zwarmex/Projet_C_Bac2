#include <stdio.h>
#include <stdlib.h>
#include "../Headers/Examen.h"
#define LENGTHARRAY(x) (sizeof(timeCircuitMS)/sizeof(*timeCircuitMS))


int main()
{
	int upperTimeMaxMS = 45000, lowerTimeMinMS = 25000, timeCircuitMS[3], grandPrice, boolClassicWeekend = 1, arrayCars[] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};
	
	for (int i = 0; i < LENGTHARRAY(timeCircuitMS); ++i)
	{
		timeCircuitMS[i] = (rand() % (upperTimeMaxMS - lowerTimeMinMS + 1)) + lowerTimeMinMS;
		printf("Temps Random S%d: %d milliseconds (%d seconds)\n", i+1, timeCircuitMS[i], MillisecondsToSeconds(timeCircuitMS[i]));
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
	printf("Voiture 1 : %d\n", arrayCars[0]);
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
