/* show-bytes - prints byte representation of data */

/* $begin show-bytes */
#include <stdio.h>
/* $end show-bytes */
#include <stdlib.h>
#include <string.h>
/* $begin show-bytes */

typedef unsigned char *byte_pointer;

void show_bytes(byte_pointer start, size_t len) {
    size_t i;
    for (i = 0; i < len; i++)
	    printf("%.2x ", start[i]); 
    printf("\n");
}

void show_int(int x) {
    show_bytes((byte_pointer) &x, sizeof(int)); 
}

void show_float(float x) {
    show_bytes((byte_pointer) &x, sizeof(float));
}

void show_pointer(void *x) {
    show_bytes((byte_pointer) &x, sizeof(void *));
}
/* $end show-bytes */


/* $begin test-show-bytes */
void test_show_bytes(int val) {
    int ival = val;
    float fval = (float) ival;
    int *pval = &ival;

    printf("(int)ival:\n");
    show_int(ival);
    printf("(float)ival:\n", ival);
    show_float(fval);
    printf("&ival:\n", ival);
    show_pointer(pval);
}
/* $end test-show-bytes */
/*
int main(int argc, char *argv[])
{
    int val = 0xeeff;

    test_show_bytes(val);
    return 0;
}
*/