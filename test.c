#include <stdio.h>
#include <unistd.h>

int main()
{
    printf("Process ID: %d\n", getpid());
    printf("Process Parent's ID: %d\n", getppid());
    return 0;
}