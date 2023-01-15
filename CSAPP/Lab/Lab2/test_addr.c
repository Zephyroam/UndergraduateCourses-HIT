#include <stdio.h>

const int x = -1190200523;

int main()
{
    float y = 131182200003105535;
    static char z[100] = "1190200523-石翔宇";
    unsigned *yp = &y;
    printf("x = %d\n", x);
    printf("y = %lf = 0x%x\n", y, *yp);
    printf("z = %s\n", z);
    return 0;
}
