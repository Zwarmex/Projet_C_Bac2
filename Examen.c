#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "Headers/Functions.h"

int main() // Add boolClassicWeekEnd in arg
{

	// Initialisation of variables
	int boolSprint = 0, boolClassicWeekEnd = 1,
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};

	// Put seed number in rand
	srand(time(NULL));
	
	// Create array of cars
	struct Car *arrayCars = CarBuilder(arrayCarsId);
	if (boolClassicWeekEnd)
	{

		printf("\nFriday's morning : Free Try\n\n");
		DoFreeTry(arrayCars[0]);
		printf("\nFriday's afternoon : Free Try\n\n");
		DoFreeTry(arrayCars[0]);
		printf("\nSaturday's morning : Free Try\n\n");
		DoFreeTry(arrayCars[0]);

	}
	
	// If cars testing
	if (!boolSprint)
	{
		
	}

	else
	{
		
	}

	FILE *pointerFileScore = fopen("Results/score.txt", "w");

	for (int i = 0; i < NUMBEROFCARS; i++)
	{

		fprintf(pointerFileScore, "Car : %d -> Turn : %d\n", arrayCars[i].id, arrayCars[i].turnTimeMS);
	}
	

	fclose(pointerFileScore);

	return 0;
}

void DoFreeTry(struct Car car)
{
	// While true continue turn testing
	int boolContinueTesting = 1, upperTimeMaxMS = 45000, lowerTimeMinMS = 25000;
	while (boolContinueTesting && !car.state)
	{

		// Reset the time  of the circuit of the car
		car.turnTimeMS = 0;
		// For each sections
		for (int i = 0; i < LENGTHARRAY(car.timeCircuitMS); ++i)
		{

			car.timeCircuitMS[i] = (rand() % (upperTimeMaxMS - lowerTimeMinMS + 1)) + lowerTimeMinMS;
			
			// sleep(MillisecondsToSeconds(car.timeCircuitMS[i]));
			
			// if the car is doing a better time 

			if (car.bestTimeCircuitMS[i] == 0 || car.bestTimeCircuitMS[i] > car.timeCircuitMS[i])
			{

				car.bestTimeCircuitMS[i] = car.timeCircuitMS[i];
				char* sectionAndSentence = malloc(sizeof("S a new best time") + sizeof(int));
				
				strcpy(sectionAndSentence, "S");
				char sectionNumber[2] = {i+1+'0'};
				strcat(sectionAndSentence, sectionNumber);
				strcat(sectionAndSentence, " new best time");

				PrintBestTimeWithText(car.id, car.bestTimeCircuitMS[i] , sectionAndSentence);
				
				free(sectionAndSentence);
			}

			// Keep the time of the circuit
			car.turnTimeMS += car.timeCircuitMS[i];
		}

		PrintBestTimeWithText(car.id, car.turnTimeMS, "Turn");
		boolContinueTesting =  rand() % 2;

		if((rand() % 15) == 1)
		{

			car = EndOfSession(car);
		}
		if((rand() % 15) == 1)
		{

			car = EnterThePits(car);
		}
	}
}