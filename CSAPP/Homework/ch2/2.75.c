#include <stdio.h>

int signed_high_prod(int x, int y)
{
    long long s = (long long)x * y;
    return s >> 32;
}

unsigned unsigned_high_prod(unsigned x, unsigned y)
{
    int m = signed_high_prod((int)x, (int)y);
    return m + x * (y >> 31) + y * (x >> 31);
}
int main()
{
    
}