#include <stdio.h>

unsigned rotate_left(unsigned x, int n)
{
    n = (n | 32) - 32;
    unsigned y = x >> (32 - n);
    x <<= n;
    return x | y;
}

int main()
{
    unsigned x = 0x12345678;
    int n = 0;
    printf("0x%x\n", rotate_left(x, n));
    return 0;
}