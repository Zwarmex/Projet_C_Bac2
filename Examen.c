#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "Headers/Functions.h"

int main()
{

	// Initialisation of variables
	int upperTimeMaxMS = 45000, lowerTimeMinMS = 25000, boolSprint = 0, 
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};

	// Put seed number in rand
	srand(time(NULL));
	
	// Create array of cars
	struct Car *arrayCars = CarBuilder(arrayCarsId);

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
				
				// sleep(MillisecondsToSeconds(arrayCars[0].timeCircuitMS[i]));
				
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

	FILE *pointerFileScore = fopen("score.txt", "w");

	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		fprintf(pointerFileScore, "Car : %d -> Turn : %d\n", arrayCars[i].id, arrayCars[i].turnTimeMS);
	}
	

	fclose(pointerFileScore);

	return 0;
}