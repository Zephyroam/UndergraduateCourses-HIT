#include <stdio.h>

int f(int x, int y, int z)
{
    int mm = x;
    return mm + y + z;
}

int main()
{
    int x, y, z;
    x = 1, y = 1, z = 2;
    printf("%d\n", f(x, y, z));
    return 0;
}