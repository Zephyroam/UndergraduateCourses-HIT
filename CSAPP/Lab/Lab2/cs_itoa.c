#include <stdio.h>
#include <stdlib.h>

char *cs_itoa(int a)
{
    char *res = malloc(sizeof(char) * 100);
    int p = 0;
    int base = 10;
    while(base < a)
        base *= 10;
    while(a)
    {
        base /= 10;
        res[p++] = a / base + '0';
        a -= a / base * base;
    }
    return res;
}

int main()
{
    int a = 12345556;
    printf("%s\n", cs_itoa(a));
    return 0;
}