#include <stdio.h>

int cs_atoi(char *ch)
{
    int res = 0;
    while((*ch) >= '0' && (*ch) <= '9')
    {
        res = res * 10 + (*ch) - '0';
        ch++;
    }
    return res;
}

int main()
{
    char a[] = "1234567";
    printf("%d\n", cs_atoi(a));
    return 0;
}