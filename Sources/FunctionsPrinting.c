void PrintScore(Car *arrayCars)
{
	FILE *pointerFileScore;
	if (!(pointerFileScore = fopen("Results/score.txt", "w")))
	{
		perror("fopen error");
		exit(EXIT_FAILURE);
	}

	// Sort the array of cars
	Car *sortedArrayCars = SortArrayCars(arrayCars);

	if(!(fprintf(pointerFileScore, "Car		S1			S2			S3			Best TT				PIT			OUT\n")))
	{
		perror("fprintf title error ");
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		Car *car = (Car *) (&sortedArrayCars[i]);
		char *arrayBuffersTime[4];
		for (int i = 0; i < 4; i++)
		{
			if(!(arrayBuffersTime[i] = malloc(sizeof(":") * 3 + sizeof(int) * 4)))
			{
				perror("malloc error ");
				exit(EXIT_FAILURE);
			}
		}
		
		if(!(fprintf(pointerFileScore, "%d		%s		%s		%s		%s		%s		%s\n", 
		car->id, returnBestTime(car->timeSectionMS[0], arrayBuffersTime[0]), returnBestTime(car->timeSectionMS[1], arrayBuffersTime[1]), 
		returnBestTime(car->timeSectionMS[2], arrayBuffersTime[2]), returnBestTime(car->bestTimeTurnMS, arrayBuffersTime[3]), 
		(car->state == 2)?"True":"False", (car->state == 1)?"True":"False")))
		{
			perror("fprintf data error ");
			exit(EXIT_FAILURE);
		}

		for (int i = 0; i < 4; i++)
		{
			free(arrayBuffersTime[i]);
		}
		
	}

	if(fclose(pointerFileScore))
	{
		perror("fclose error ");
		exit(EXIT_FAILURE);
	}
}