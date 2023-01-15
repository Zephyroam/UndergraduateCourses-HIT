#include <stdio.h>
#include "show-bytes.c"
int is_little_endian()
{
    int x = 0xff;
    byte_pointer p = (byte_pointer) &x;
    if(p[0] == 0xff)
        return 1;
    else   
        return 0;
}
int main()
{
    printf("%d\n", is_little_endian());
    return 0;
}