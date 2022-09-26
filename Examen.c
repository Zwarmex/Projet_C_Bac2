#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "Headers/Functions.h"
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
	int upperTimeMaxMS = 45000, lowerTimeMinMS = 25000, boolSprint = 0, 
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};

	// Put seed number in rand
	srand(time(NULL));
	
	// Create array of cars
	struct Car arrayCars[] = {CarBuilder(arrayCarsId)};

	// If cars testing
	if (!boolSprint)
	{
		// While true continue turn testing
		int boolContinueTesting = 1;
		while (boolContinueTesting && !arrayCars[0].state)
		{

			// Reset the time  of the circuit of the car
			arrayCars[0].turnTimeMS = 0;
			// For each sections
			for (int i = 0; i < LENGTHARRAY(arrayCars[0].timeCircuitMS); ++i)
			{

				arrayCars[0].timeCircuitMS[i] = (rand() % (upperTimeMaxMS - lowerTimeMinMS + 1)) + lowerTimeMinMS;
				// if the car is doing a better time 
				if (arrayCars[0].bestTimeCircuitMS[i] == 0 || arrayCars[0].bestTimeCircuitMS[i] > arrayCars[0].timeCircuitMS[i])
				{

					arrayCars[0].bestTimeCircuitMS[i] = arrayCars[0].timeCircuitMS[i];
					char* sectionAndSentence = malloc(sizeof("S a new best time") + sizeof(int));
					
					strcpy(sectionAndSentence, "S");
					char sectionNumber[] = {i+1+'0'};
					strcat(sectionAndSentence, sectionNumber);
					strcat(sectionAndSentence, " new best time");

					PrintBestTimeWithText(arrayCars[0].id, arrayCars[0].bestTimeCircuitMS[i] , sectionAndSentence);
					
					free(sectionAndSentence);
				}

				// Keep the time of the circuit
				arrayCars[0].turnTimeMS += arrayCars[0].timeCircuitMS[i];
			}

			PrintBestTimeWithText(arrayCars[0].id, arrayCars[0].turnTimeMS, "Turn");
			boolContinueTesting =  rand() % 2;

			if((rand() % 15) == 1)
			{

				arrayCars[0] = EndOfSession(arrayCars[0]);
			}
			if((rand() % 15) == 1)
			{

				arrayCars[0] = EnterThePits(arrayCars[0]);
			}
		}
	}

	else
	{
		
	}

	

	return 0;
}


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