#include <stdio.h>

int int_size_is_32()
{
    int x = 1 << 31;
    int y = (~x) + 1;
    return (x == y) && x;
}

int main()
{
    printf("%d\n", int_size_is_32());
    return 0;
}