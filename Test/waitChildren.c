#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <fcntl.h> 
#include <unistd.h>
#include <wait.h>

int main(int argc, char const *argv[])
{
    int forkId;
    for (int i = 0; i < 3; i++)
    {
        forkId = fork();
        if (forkId == 0)
        {
            sleep(i);
            printf("child exiting\n");
            return 0;
        }
        
    }
    int waitRespons, waitStatus;
    while ((waitRespons = waitpid(-1, &waitStatus, WNOHANG)) !=-1)
    {
        printf("parent waiting\n");
        usleep(100000);
    }
    printf("parent exiting\n");
    return 0;
}
