#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

char *returnBest(int MS, char *buff);

int main(int argc, char const *argv[])
{
    // printf("%s\n", returnBest(45000));
    char *buff  = malloc(sizeof(":") * 3 + sizeof(int) * 4);
    printf("%s\n%s", returnBest(99567, buff), returnBest(14789, buff));
    free(buff);
    return 0;
}

char *returnBest(int timeMS, char *buff)
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
        char *buffReturned;
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
