#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include "Headers/Functions.h"

int main() // Add boolClassicWeekEnd in arg
{

	// Initialisation of variables
	int boolSprint = 0, boolClassicWeekEnd = 1, pidFork,
	arrayCarsId[NUMBEROFCARS] = {44, 63, 1, 11, 55, 16, 4, 3, 14, 31, 10, 22, 5, 18, 6, 23, 77, 24, 47, 9};

	// Put seed number in rand
	srand(time(NULL));
	
	// Create array of cars
	struct Car *arrayCars = CarBuilder(arrayCarsId);
	
	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		pidFork = fork();

		// Fork Error
		if (pidFork == -1)
        {
			perror("fork error");
            exit(EXIT_FAILURE);
		}
		
		// Child (a car)
		if (pidFork == 0)
        {
			if (boolClassicWeekEnd)
			{
				printf("Friday's morning : Free Try\n\n");
				DoFreeTry(&arrayCars[i]);
				printf("\nFriday's afternoon : Free Try\n\n");
				DoFreeTry(&arrayCars[i]);
				printf("\nSaturday's morning : Free Try\n\n");
				DoFreeTry(&arrayCars[i]);
			}	

			// If cars testing
			if (!boolSprint)
			{
				
			}

			else
			{
				
			}

			// Child have to not make another child
			break;
		}
	}

	FILE *pointerFileScore = fopen("Results/score.txt", "w");

	for (int i = 0; i < NUMBEROFCARS; i++)
	{

		fprintf(pointerFileScore, "Car : %d -> Turn : %d\n", arrayCars[i].id, arrayCars[i].turnTimeMS);
	}
	

	fclose(pointerFileScore);

	return 0;
}