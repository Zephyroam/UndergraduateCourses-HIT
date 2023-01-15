#include <stdio.h>

int main()
{
    int x = 0xeeffff00;
    int output = !(x - 0xffffffff) | (!x) | !((x & 0xff) - 0xff) | !(x >> 24);
    printf("%d", output);
    return 0;
}