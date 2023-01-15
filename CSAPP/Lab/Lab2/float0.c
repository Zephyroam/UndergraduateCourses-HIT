#include <stdio.h>

int main()
{
    float x = 1.0;
    printf("x / 0 = %f\n", x / 0);
    printf("x / 1e-100 = %f\n", x / 1e-100);
    return 0;
}