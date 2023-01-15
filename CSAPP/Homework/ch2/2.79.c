#include <stdio.h>
#include <assert.h>
int mul3div4(int x)
{
    return (x >> 1) + (x >> 2) + ((x & 0x3) == 0x3);
}

int main()
{
    
    for (int x = 0; x < 99999999; x++)
        assert(mul3div4(x) == x * 3 / 4);
    return 0;
}