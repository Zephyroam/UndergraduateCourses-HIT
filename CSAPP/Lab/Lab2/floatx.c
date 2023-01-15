#include <stdio.h>

int main()
{
    float positive_zero;
    float negative_zero;
    float minimum_positive_float;
    float maximum_positive_float;
    float minimum_positive_normalized_float;
    float positive_infinity;
    float not_a_number;

    unsigned *pointer_positive_zero = &positive_zero;
    unsigned *pointer_negative_zero = &negative_zero;
    unsigned *pointer_minimum_positive_float = &minimum_positive_float;
    unsigned *pointer_maximum_positive_float = &maximum_positive_float;
    unsigned *pointer_minimum_positive_normalized_float = &minimum_positive_normalized_float;
    unsigned *pointer_positive_infinity = &positive_infinity;
    unsigned *pointer_not_a_number = &not_a_number;

    *pointer_positive_zero = 0;
    *pointer_negative_zero = 0x80000000;
    *pointer_minimum_positive_float = 0x00000001;
    *pointer_maximum_positive_float = 0x7f7fffff;
    *pointer_minimum_positive_normalized_float = 0x00800000;
    *pointer_positive_infinity = 0x7f800000;
    *pointer_not_a_number = 0x7f800001;
    printf("%f\t0x%.8x\n", positive_zero, *pointer_positive_zero);
    printf("%f\t0x%.8x\n", negative_zero, *pointer_negative_zero);
    printf("%f\t0x%.8x\n", minimum_positive_float, *pointer_minimum_positive_float);
    printf("%f\t0x%.8x\n", maximum_positive_float, *pointer_maximum_positive_float);
    printf("%f\t0x%.8x\n", minimum_positive_normalized_float, *pointer_minimum_positive_normalized_float);
    printf("%f\t0x%.8x\n", positive_infinity, *pointer_positive_infinity);
    printf("%f\t0x%.8x\n", not_a_number, *pointer_not_a_number);
    return 0;
}