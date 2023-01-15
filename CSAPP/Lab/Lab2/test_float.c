#include <stdio.h>


int main()
{
    float a = -1.1;
    unsigned *ap = &a;
    unsigned char *app = &a;
    printf("%f = 0x%x\n", a, *ap);
    return 0;
}