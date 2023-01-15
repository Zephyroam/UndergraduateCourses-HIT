#include <stdio.h>
#include <stdlib.h>

char *cs_ftoa(float a)
{
    char *res = malloc(sizeof(char) * 100);
    int p = 0;
    float base = 10;
    while(base < a)
        base *= 10;
    while(a > 1)
    {
        base /= 10;
        printf("%f\n", a / base);
        res[p++] = (int)(a / base) + '0';
        a -= (int)(a / base) * base;
    }

    res[p++] = '.';
    while(fabs(a - (int)a) > 1e-7)
        a *= 10;
    while(base < a)
        base *= 10;
    while(a > 1)
    {
        base /= 10;
        res[p++] = (int)(a / base) + '0';
        a -= (int)(a / base) * base;
    }
    return res;
}

int main()
{
    float a = 123.4;
    printf("%f\n", a);
    printf("%s\n", cs_ftoa(a));
    return 0;
}