#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const char *semName = "asdfsd";
void parent(sem_t *sem_id)
{    
    printf("Parent  : Wait for Child to Print\n");

    if (sem_wait(sem_id) < 0)
    {
        printf("Parent  : [sem_wait] Failed\n");
    }

    printf("Parent  : Child Printed! \n");

    if (sem_post(sem_id) < 0)
    {
        printf("Child   : [sem_post] Failed \n");
    }
}

// void child(sem_t *sema)
// {   
//     sleep(1);

//     if (sema == SEM_FAILED)
//     {
//         perror("Child   : [sem_open] Failed\n"); return;        
//     }

//     printf("Child   : I am done! Release Semaphore\n");

//     if (sem_post(sema) < 0)
//     {
//         printf("Child   : [sem_post] Failed \n");
//     }
// }
int main(int argc, char *argv[])
{
    pid_t pid;
    pid = fork();
    if (pid < 0)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    if (!pid)
    {   
        // sem_t *sem_id = sem_open(semName, O_CREAT, 0600, 0);
        // for (int i = 0; i < 3; i++)               
        //     child(sem_id);
    }
    else
    {
        sem_t *sem_id = sem_open(semName, O_CREAT, 0600, 1);
        if (sem_id == SEM_FAILED)
        {
            perror("Parent  : [sem_open] Failed\n");
        }

        int status;
        for (size_t i = 0; i < 3; i++)   
        {           
            sleep(1);     
            parent(sem_id);
        }
        if (sem_close(sem_id) != 0)
        {
            perror("Parent  : [sem_close] Failed\n");
        }
        if (sem_unlink(semName) < 0)
        {
            printf("Parent  : [sem_unlink] Failed\n");
        }
    }
    return 0;
}