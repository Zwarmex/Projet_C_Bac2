void PrintScore(Car *arrayCars)
{
	// Sort the array of cars
	Car *sortedArrayCars = SortArrayCars(arrayCars);

    // Put the cursor on row 2 column 1
    printf("\033c\033[4m\033[47m\033[30mCar\t\tS1\t\tS2\t\tS3\t\tBest TT\t\t\tPIT\t\tOUT\033[m\033[2;H");

	for (int i = 0; i < NUMBEROFCARS; i++)
	{
		Car *car = (Car *) (&sortedArrayCars[i]);
		char *arrayBuffersTime[4];
        int row = 2;
		for (int i = 0; i < 4; i++)
		{
			if(!(arrayBuffersTime[i] = malloc(sizeof(":") * 3 + sizeof(int) * 4)))
			{
				perror("malloc error ");
				exit(EXIT_FAILURE);
			}
		}
		
		printf("%d		%s		%s		%s		%s		%s		%s\r\n", 
		car->id, returnBestTime(car->timeSectionMS[0], arrayBuffersTime[0]), returnBestTime(car->timeSectionMS[1], arrayBuffersTime[1]), 
		returnBestTime(car->timeSectionMS[2], arrayBuffersTime[2]), returnBestTime(car->bestTimeTurnMS, arrayBuffersTime[3]), 
		(car->state == 2)?"True":"False", (car->state == 1)?"True":"False");

		for (int i = 0; i < 4; i++)
		{
			free(arrayBuffersTime[i]);
		}
	}
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
        char hoursTime[3], hoursTmp[3];
        len = sprintf(hoursTmp, "%d", hours);
        while (len < 2)
        {
            len = sprintf(hoursTime, "%s%s", "0", hoursTmp);
        }       

        strcat(buff, hoursTime);
        strcat(buff, ":");

    }

    if (minutes || hours)
    {
        char minutesTime[3], minutesTmp[3];
        len = sprintf(minutesTmp, "%d", minutes);
        while (len < 2)
        {
            len = sprintf(minutesTime, "%s%s", "0", minutesTmp);
        }

        strcat(buff, minutesTime);
        strcat(buff, ":");
    }

    if (seconds || minutes || hours)
    {
        char secondsTime[3], zeros[] = "0";
        len = sprintf(secondsTime, "%d", seconds);
        char * buffTmp = len < 2 ? zeros + 2 - len : zeros;
        sprintf(buffTmp, "%s", secondsTime);

        strcat(buff, buffTmp);
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

void WhileChildrenAreBusy(void)
{
    if(sem_wait(semaParentId) < 0)
    {
        perror("sem_wait parent error ");
        exit(EXIT_FAILURE);
    }

    if (sem_wait(semaChildId) < 0)
    {
        perror("sem_wait parent error ");
        exit(EXIT_FAILURE);
    }

    int value;

    if (sem_getvalue(semaParentId, &value) < 0)
    {
        perror("sem_getvalue parent error ");
        exit(EXIT_FAILURE);
    }
    while (value > 0)
    {
        if(sem_wait(semaParentId) < 0)
        {
            perror("sem_wait parent error ");
            exit(EXIT_FAILURE);
        }

        if (sem_getvalue(semaParentId, &value) < 0)
        {
            perror("sem_getvalue parent error ");
            exit(EXIT_FAILURE);
        }
    }

    PrintScore(shMem);

    if (sem_post(semaChildId) < 0)
    {
        perror("sem_post parent error ");
        exit(EXIT_FAILURE);
    }
}