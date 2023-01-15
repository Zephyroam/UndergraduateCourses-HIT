#include <stdio.h>
#include <stdbool.h>

typedef unsigned char *byte_pointer;
bool isLittleEndian()
{
    int x = 0xab;
    byte_pointer start = (byte_pointer)&x;
    return *start == 0xab;
}

int main()
{
    printf("%d\n", isLittleEndian());
    return 0;
}