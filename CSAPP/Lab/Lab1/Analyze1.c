#include <stdio.h>

int sum(int a[], unsigned int len)
{
    int i, sum = 0;
    for (i = 0; i <= len - 1; i++)
        sum += a[i];
    return sum;
}

int main()
{
    int a[5] = {1, 2, 3, 4, 5};
    printf("%d\n", sum(a, 0));
    return 0;
}