#include <stdio.h>

typedef unsigned packed_t;

int xbyte(packed_t word, int bytenum)
{
    int x = word >> (bytenum << 3);
    int y = (x - (x >> 8 << 8)) << 24 >> 24;
    return y;
}

int main()
{
    packed_t word = 0xffffffff;
    int bytenum = 3;
    printf("0x%x\n", xbyte(word, bytenum));
    return 0;
}