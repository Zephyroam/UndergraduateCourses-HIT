#include <stdio.h>

int any_odd_one(unsigned x)
{
    unsigned y = 0x55555555;
    return (x & y) && 1;
}

int main()
{
    unsigned x = 3;
    printf("%d\n", any_odd_one(x));
    return 0;
}