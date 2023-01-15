#include <stdio.h>

int int_shifts_are_arithmetic()
{
    int x = 0xffffffff;
    int y = x >> 8;
    return !(y - x);
}

int main()
{
    printf("%d\n", int_shifts_are_arithmetic());
    return 0;
}