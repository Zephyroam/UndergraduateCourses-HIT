#include <stdio.h>

#define MAX_SIZE 1000

void good_echo()
{
    char a[MAX_SIZE];
    while(1)
    {
        char *now = fgets(a, MAX_SIZE, stdin);
        if (now == NULL)
            break;
        printf("%s", a);
    }
}