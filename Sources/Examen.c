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

	// Initialisation of variables
	int upperTimeMaxMS = 45000, lowerTimeMinMS = 25000, arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9},
	sprint = 0;

	// Put seed number in rand
	srand(time(NULL));
	
	// Create array of cars
	struct Car arrayCars[] = {CarBuilder(arrayCarsId)};

	// If cars testing
	if (!sprint)
	{
		// While true continue testing
		int boolContinueTesting = 1;
		while (boolContinueTesting)
		{

			// Reset the time  of the circuit of the car
			printf("La voiture test le circuit.\n");
			arrayCars[0].turnTimeMS = 0;
			for (int i = 0; i < LENGTHARRAY(arrayCars[0].timeCircuitMS); ++i)
			{

				arrayCars[0].timeCircuitMS[i] = (rand() % (upperTimeMaxMS - lowerTimeMinMS + 1)) + lowerTimeMinMS;			
				if (arrayCars[0].bestTimeCircuitMS[i] == 0 || arrayCars[0].bestTimeCircuitMS[i] > arrayCars[0].timeCircuitMS[i])
				{

					arrayCars[0].bestTimeCircuitMS[i] = arrayCars[0].timeCircuitMS[i];
					char* sectionAndSentence = malloc(sizeof("the section has recorded a new best time") + sizeof(int));
					
					strcpy(sectionAndSentence, "the section ");
					char sectionNumber[] = {i+1+'0'};
					strcat(sectionAndSentence, sectionNumber);
					strcat(sectionAndSentence, " has recorded a new best time");

					PrintCarTurnTiming(arrayCars[0].id, arrayCars[0].bestTimeCircuitMS[i] , sectionAndSentence);
					
					free(sectionAndSentence);
				}

				arrayCars[0].turnTimeMS += arrayCars[0].timeCircuitMS[i];
			}

			PrintCarTurnTiming(arrayCars[0].id, arrayCars[0].turnTimeMS, "time for the whole turn");
			// boolContinueTesting =  rand() % 2;
			boolContinueTesting = 0;
		}
	}

	else
	{
		
	}
	
	
	

	return 0;
}

void PrintCarTurnTiming(int id, int timeMS, char sent[])
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
		arrayCars[i].turnTimeMS = 0;
		arrayCars[i].position = 0; // For later
		arrayCars[i].state = 0; // Ready to starting
		for (int j = 0; j < LENGTHARRAY(arrayCars[i].timeCircuitMS); j++)
		{
			arrayCars[i].timeCircuitMS[j] = 0;
			arrayCars[i].bestTimeCircuitMS[j] = 0;
		}
		
	}

	return *arrayCars;
}