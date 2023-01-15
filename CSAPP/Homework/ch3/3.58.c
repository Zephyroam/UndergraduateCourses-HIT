#include <stdio.h>

long decode2(long x, long y, long z)
{
    long res;
    y -= z;
    x *= y;
    res = y;
    res <<= 63;
    res >>= 63;
    res ^= x;
    return res;
}

int main()
{
    long x, y, z;
    printf("%ld\n", decode2(x, y, z));
    return 0;
}