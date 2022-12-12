

void PrintScore(Car *arrayCars, int size)
{
	// Sort the array of cars
	Car *sortedArrayCars = SortArrayCars(arrayCars);

    // Put the cursor on row 2 column 1
    printf("\033c\033[4m\033[47m\033[30mCar\t\tS1\t\tS2\t\tS3\t\tBest TT\t\t\tPIT\t\tOUT\t\tDIFF\033[m\033[2;H");

	for (int i = 0; i < size; i++)
	{
		Car *car = (Car *) (&sortedArrayCars[i]);
		char *arrayBuffersTime[CAR_TIME_BUFFER];
		for (int j = 0; j < CAR_TIME_BUFFER; j++)
		{
			if(!(arrayBuffersTime[j] = malloc(sizeof(":") * 3 + sizeof(int) * 4)))
			{
				perror("malloc error ");
				exit(EXIT_FAILURE);
			}
		}
		if (i != 0)
        {
            Car *previousCar = (Car *) (&sortedArrayCars[i - 1]);
            printf("%d		%s		%s		%s		%s		%s		%s      +%s\r\n",
               car->id, returnBestTime(car->timeSectionMS[0], arrayBuffersTime[0]),
               returnBestTime(car->timeSectionMS[1], arrayBuffersTime[1]),
               returnBestTime(car->timeSectionMS[2], arrayBuffersTime[2]),
               returnBestTime(car->bestTimeTurnMS, arrayBuffersTime[3]),
               (car->state == 2) ? "True" : "False", (car->state == 1) ? "True" : "False",
               returnBestTime(car->bestTimeTurnMS - previousCar->bestTimeTurnMS, arrayBuffersTime[4]));
        }
        else
        {
            printf("%d		%s		%s		%s		%s		%s		%s\r\n",
                car->id, returnBestTime(car->timeSectionMS[0], arrayBuffersTime[0]),
                returnBestTime(car->timeSectionMS[1], arrayBuffersTime[1]),
                returnBestTime(car->timeSectionMS[2], arrayBuffersTime[2]),
                returnBestTime(car->bestTimeTurnMS, arrayBuffersTime[3]),
                (car->state == 2) ? "True" : "False", (car->state == 1) ? "True" : "False");
        }

		for (int j = 0; j < CAR_TIME_BUFFER; j++)
		{
			free(arrayBuffersTime[j]);
		}
	}

    char *arrayBuffersBestTime[BEST_TIME_BUFFER];
    int bestS1 = INT_MAX, bestS2 = INT_MAX, bestS3 = INT_MAX;
    for (int i = 0; i < size; ++i)
    {
        Car *car = (Car *) (&sortedArrayCars[i]);
        if (bestS1 > car->bestTimeSectionMS[0])
        {
            bestS1 = car->bestTimeSectionMS[0];
        }
        if (bestS2 > car->bestTimeSectionMS[1])
        {
            bestS2 = car->bestTimeSectionMS[1];
        }
        if (bestS3 > car->bestTimeSectionMS[2])
        {
            bestS3 = car->bestTimeSectionMS[2];
        }
    }
    for (int i = 0; i < BEST_TIME_BUFFER; i++)
    {
        if(!(arrayBuffersBestTime[i] = malloc(sizeof(":") * 3 + sizeof(int) * 4)))
        {
            perror("malloc error ");
            exit(EXIT_FAILURE);
        }
    }

    printf("Best TT : %s\nBest S1 : %s\nBest S2 : %s\nBest S2 : %s\n",
           returnBestTime(sortedArrayCars[0].bestTimeTurnMS, arrayBuffersBestTime[0]),
           returnBestTime(bestS1, arrayBuffersBestTime[1]),
           returnBestTime(bestS2, arrayBuffersBestTime[2]),
           returnBestTime(bestS3, arrayBuffersBestTime[3]));
}

char *returnBestTime(int timeMS, char *buff)
{
    int hours = ((timeMS / 1000) / 3600), len,
	minutes = ((timeMS / 1000) - (hours * 3600)) / 60, 
	seconds = ((timeMS / 1000) - (hours * 3600) - (minutes * 60)),
	milliseconds = ((timeMS) - (hours * 3600 * 1000) - (minutes * 60 *1000) - (seconds * 1000));

    // initialisation of the buff
    strcpy(buff, "");

    if (hours)
    {
        char hoursTime[3];
        sprintf(hoursTime, "%02d", hours);

        strcat(buff, hoursTime);
        strcat(buff, ":");
    }

    if (minutes || hours)
    {
        char minutesTime[3];
        sprintf(minutesTime, "%02d", minutes);

        strcat(buff, minutesTime);
        strcat(buff, ":");
    }

    if (seconds || minutes || hours)
    {
        char secondsTime[3];
        sprintf(secondsTime, "%02d", seconds);
        
        strcat(buff, secondsTime);
        strcat(buff, ":");
    }

    char millisecondsTime[4];
    len = sprintf(millisecondsTime, "%d", milliseconds);
    while (len < 3)
    {
        len = sprintf(millisecondsTime, "%s%s", millisecondsTime, "0");
    }

    strcat(buff, millisecondsTime);

    return buff;
}

// Function to perform Sort array of cars
Car *SortArrayCars(Car *arrayCars)
{
	static Car copyArrayCars[NUMBER_OF_CARS];
	// Copy the array of cars into a new one.
	for (int i = 0; i < NUMBER_OF_CARS; i++)
	{
		copyArrayCars[i] = arrayCars[i];
	}
	
    // One by one move boundary of unsorted subarray
    for (int i = 0; i < NUMBER_OF_CARS - 1; i++) 
	{
        // Find the minimum element in unsorted array
        int minId = i;
        for (int j = i + 1; j < NUMBER_OF_CARS; j++)
        {
            int bestTimeJ = (copyArrayCars[j].bestTimeTurnMS == 0) ? INT_MAX : copyArrayCars[j].bestTimeTurnMS;
            int bestTimeMinId = (copyArrayCars[minId].bestTimeTurnMS == 0) ? INT_MAX : copyArrayCars[minId].bestTimeTurnMS;
            if (bestTimeJ < bestTimeMinId)
            {
                minId = j;
            }
        }

        // Swap the found minimum element with the actual element
		Car temp = copyArrayCars[minId];
		copyArrayCars[minId] = copyArrayCars[i];
		copyArrayCars[i] = temp;
    }
	return copyArrayCars;
}