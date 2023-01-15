#include <stdio.h>
#include "show-bytes.c"

void show_short(short x)
{
    show_bytes((byte_pointer) &x, sizeof(short));
}
void show_long(long x)
{
    show_bytes((byte_pointer) &x, sizeof(long));
}
void show_double(double x)
{
    show_bytes((byte_pointer) &x, sizeof(double));
}
void test_show_bytes_new(short val) {
    long lval = val;
    double dval = val;

    printf("(short)sval:\n", val);
    show_short(val);
    printf("(long)lval:\n", lval);
    show_long(lval);
    printf("(double)dval:\n", dval);
    show_double(dval);
}
/*
int main()
{
    short val = 6454;
    test_show_bytes_new(val);
    return 0;
}
*/