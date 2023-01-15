#include <stdio.h>

long switch_prob(long x, long n)
{
    long result = x;
    switch(n)
    {
        case 60:
            result = x * 8;
            break;
        case 64:
            result = x * 15;
            x = result;
        case 65:
            x *= x;
        case 61:
            result = x + 0x4b;
            break;
        case 62:
            result = x * 8;
            break;
        case 63:
            result = x >> 3;
            break;
    }
    return result;
}