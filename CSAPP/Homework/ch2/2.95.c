#include<stdio.h>

typedef unsigned float_bits;

float_bits float_half(float_bits f)
{
    float_bits exp = (f >> 23) & 0xff;
    float_bits frac = f & 0x7fffff;
    float_bits ou = (exp & 0x3) == 0x3;
    if (exp == 0)
    {
        frac >>= 1;
        frac += ou;
    }
    else if (exp == 1)
    {
        frac >>= 1;
        frac += ou + (1 << 22);
    }
    else if (exp != 255)
        exp--;
    return (f & 0x80000000) | (exp << 23) | frac;
}

int main()
{
    float f = 45.3;
    float_bits *fp = (float_bits *)&f;
    float_bits fb_res = float_half(*fp);
    float *res = (float *)&fb_res;
    printf("%f", *res);
    return 0;
}