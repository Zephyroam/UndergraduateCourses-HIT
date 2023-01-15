#include <stdio.h>

int main()
{
    int x = 0x89abcdef, y = 0x76543210;
    int output = x & 0xff | (y & ~0xff);
    printf("0x%x\n", output);
    return 0;
}