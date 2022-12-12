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

int main(int argc, char const *argv[])
{
    int pid = fork();
    srand(time(NULL) ^ (getpid()<<16));
    if (!pid)
    {
        for (int i = 0; i < 3; i++)
        {
            printf("n child : %d\n", rand() % (50 - 10 + 1) + 10);
        }
        return 0;
    }
    
    for (int i = 0; i < 3; i++)
    {
        printf("n parent: %d\n", rand() % (50 - 10 + 1) + 10);
    }
    
    return 0;
}
