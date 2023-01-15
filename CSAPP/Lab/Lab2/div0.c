#include <stdio.h>

int main()
{
    int x = 1;
    printf("1 / 0 = %d", x / 0);
    printf("1 / 1e-30 = %f", x / 1e-30);
    return 0;
}