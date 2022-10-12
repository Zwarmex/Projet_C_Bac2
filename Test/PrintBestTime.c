#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>

void PrintBestTimeWithText(int timeMS);



int main()
{
    srand(time(NULL));
    for (int i = 0; i < 4; i++)
    {
        PrintBestTimeWithText((rand() % (45000 - 25000 + 1) + 25000));
    }
    
    return 0;
}

void PrintBestTimeWithText(int timeMS)
{
	int hours = ((timeMS / 1000) / 3600), 
	minutes = ((timeMS / 1000) - (hours * 3600)) / 60, 
	seconds = ((timeMS / 1000) - (hours * 3600) - (minutes * 60)),
	milliseconds = (((timeMS) - (hours * 3600) - (minutes * 60)) - (seconds * 1000));

    if (hours)
    {
        printf("%d:%d:%d:%d\n", hours, minutes, seconds, milliseconds);
    }

    else if (minutes)
    {
       	printf("%d:%d:%d\n", minutes, seconds, milliseconds);
    }

    else if (seconds)
    {
        printf("%d:%d\n", seconds, milliseconds);
    }

    else
    {
        printf("%d\n", milliseconds);
    }
}