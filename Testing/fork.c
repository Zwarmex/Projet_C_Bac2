#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#define E 5

int main()
{
    for (int i = 0; i < E; i++)
    {
        int pid = fork();
        if (pid == 0)
        {   
            printf("%d         ", pid);
            break;
        }
    }
    printf("%d\n", getpid());
    // int x = getpid();
    // printf("getpid : %d              pid[i] : %d\n", x, pid[i]);
    // if (pid[i] == x)
    // {
    //     // printf("%d\n", pid[i]);
    // }
}