#include <stdio.h>

float cs_atof(char *ch)
{
    float res = 0;
    float base = 10;
    while(((*ch) >= '0' && (*ch) <= '9') || (*ch) == '.')
    {
        if((*ch) == '.' || base != 10)
        {
            base /= 10;
            if(base != 1)
                res += ((*ch) - '0') * base;
        }
        else
            res = res * 10 + (*ch) - '0';
        ch++;
    }
    return res;
}

int main()
{
    char a[] = "123.45";
    printf("%f\n", cs_atof(a));
    return 0;
}