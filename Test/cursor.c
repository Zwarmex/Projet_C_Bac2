#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[])
{
    printf("test\033[1;1HTest\n");
    return 0;
}
