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

		printf("Friday's morning : Free Try\n\n");
		DoFreeTry(&arrayCars[0]);
		printf("\nFriday's afternoon : Free Try\n\n");
		DoFreeTry(&arrayCars[0]);
		printf("\nSaturday's morning : Free Try\n\n");
		DoFreeTry(&arrayCars[0]);

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