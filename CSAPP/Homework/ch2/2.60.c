#include <stdio.h>

unsigned replace_byte(unsigned x, int i, unsigned char b)
{
    x = ~(0xff << (i * 8)) & x;
    x = (b << (i * 8)) | x;
    return x;
}

int main()
{
    unsigned x = 0x12345678;
    int i = 2;
    unsigned char b = 0xab;
    x = replace_byte(x, i, b);
    printf("0x%x", x);
    return 0;
}