#include <stdio.h>

int cpuWordSize()
{
    return sizeof(void *) << 3;
}
int main()
{
    printf("%d\n", cpuWordSize());
    return 0;
}