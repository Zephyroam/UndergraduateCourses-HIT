#include <stdio.h>

int lower_one_mask(int n)
{
    int m = n != 32;
    n = (n | 32) - 32;
    int x = ~(~(m << n) + 1);
    return x;
}

int main()
{
    int n = 17;
    printf("0x%x\n", lower_one_mask(n));
    return 0;
}