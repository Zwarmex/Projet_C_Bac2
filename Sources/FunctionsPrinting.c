void PrintScore(Car *arrayCars)

{
	// FILE *pointerFileScore;
	// if (!(pointerFileScore = fopen("Results/score.txt", "w")))
	// {
	// 	perror("fopen error");
	// 	exit(EXIT_FAILURE);
	// }

	// Sort the array of cars
	Car *sortedArrayCars = SortArrayCars(arrayCars);

	if(!(printf("Car		S1		S2		S3		Best TT		        PIT		OUT\n")))
	{
		perror("fprintf title error ");
		exit(EXIT_FAILURE);
	}
    printf("\033[2J");

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
		
		if(!(printf("%d		%s		%s		%s		%s		%s		%s\n", 
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
	// if(fclose(pointerFileScore))
	// {
	// 	perror("fclose error ");
	// 	exit(EXIT_FAILURE);
	// }
}

char *returnBestTime(int timeMS, char *buff)
{
    int hours = ((timeMS / 1000) / 3600), 
	minutes = ((timeMS / 1000) - (hours * 3600)) / 60, 
	seconds = ((timeMS / 1000) - (hours * 3600) - (minutes * 60)),
	milliseconds = ((timeMS) - (hours * 3600 * 1000) - (minutes * 60 *1000) - (seconds * 1000));

    if (hours)
    {
        char hoursTime[3];
        char minutesTime[3];
        char secondsTime[3];
        char millisecondsTime[4];
        sprintf(hoursTime, "%d", hours);
        sprintf(minutesTime, "%d", minutes);
        sprintf(secondsTime, "%d", seconds);
        sprintf(millisecondsTime, "%d", milliseconds);

        strcpy(buff, hoursTime);
        strcat(buff, ":");
        strcat(buff, minutesTime);
        strcat(buff, ":");
        strcat(buff, secondsTime);
        strcat(buff, ":");
        strcat(buff, millisecondsTime);

    }

    else if (minutes)
    {
        char minutesTime[3];
        char secondsTime[3];
        char millisecondsTime[4];
        sprintf(minutesTime, "%d", minutes);
        sprintf(secondsTime, "%d", seconds);
        sprintf(millisecondsTime, "%d", milliseconds);

        strcpy(buff, minutesTime);
        strcat(buff, ":");
        strcat(buff, secondsTime);
        strcat(buff, ":");
        strcat(buff, millisecondsTime);
    }

    else if (seconds)
    {
        char secondsTime[3];
        char millisecondsTime[4];
        sprintf(secondsTime, "%d", seconds);
        sprintf(millisecondsTime, "%d", milliseconds);

        strcpy(buff, secondsTime);
        strcat(buff, ":");
        strcat(buff, millisecondsTime);
    }

    else
    {
        char millisecondsTime[4];
        sprintf(millisecondsTime, "%d", milliseconds);

        strcpy(buff, millisecondsTime);
    }

    return buff;
}

