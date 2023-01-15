#include <stdio.h>

unsigned srl(unsigned x, int k)
{
    unsigned xsra = (int)x >> k;
    unsigned y = (~((x & 0x80000000) && 1)) + 1;
    return xsra ^ (y << (32 - k)) ;
}
int sra(int x, int k)
{
    int xsrl = (unsigned)x >> k;
    unsigned y = (~((x & 0x80000000) && 1)) + 1;
    return xsrl ^ (y << (32 - k));
}

int main()
{
    int x = 0x0fffffff, k = 3;
    unsigned ux = x;
    printf("%x %x\n", srl(ux, k), ux >> k);
    printf("%x %x\n", sra(x, k), x >> k);
    return 0;
}