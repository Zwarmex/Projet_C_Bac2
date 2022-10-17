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
    int shmId, shmSize = sizeof(int), *shMem;
    if((shmId = shmget(IPC_PRIVATE, shmSize, IPC_CREAT | 0775)) < 0)
	{
		perror("shmget error ");
		exit(EXIT_FAILURE);
	} 

    int forkId = fork();

    if ((shMem = (int *) shmat(shmId, NULL, 0)) < 0)
    {
        perror("shmat error ");
        exit(EXIT_FAILURE);
    }

    if (forkId == 0)
    {
        // Get the sh m from the id
		
        int data = 2;

        memcpy(&shMem, &data, sizeof(data));

        if((shmdt(shMem)) < 0)
        {
            perror("shmdt child error ");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }  
    else
    {
        // Get the sh m from the id

        int data = 1;

        sleep(1);
        
        memcpy(&shMem, &data, sizeof(data));

        sleep(1);

        printf("%d\n", shMem);

        if((shmdt(shMem)) < 0)
        {
            perror("shmdt parent error ");
            exit(EXIT_FAILURE);
        }

        if((shmctl(shmId, IPC_RMID, 0)) > 0)
        {
            perror("shmctl parent error ");
            exit(EXIT_FAILURE);
        }
        
    }


    return 0;
}
