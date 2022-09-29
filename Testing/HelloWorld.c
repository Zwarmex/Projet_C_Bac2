#include <stdio.h>
#include <stdlib.h>


int *getArray()
{
    static int arr[] = {0, 9, 9, 8, 8, 7, 7};
    return arr;
}


int main()
{
    /* code */

    int *array = getArray();

    for (int i = 0; i < 7; i++)
    {
        printf("%d\n", array[i]);
    }
    

    return 0;
}

