#include <stdio.h>

void copy_int(int val, void *buf, size_t maxbytes)
{
    if (maxbytes - sizeof(val) >= 0)
        memcpy(buf, (void *) &val, sizeof(val));
}

int main()
{
    
    return 0;
}