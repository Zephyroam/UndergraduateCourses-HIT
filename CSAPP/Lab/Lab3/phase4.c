#include <stdio.h>

int func4(int x, int y)
{
    if(x == 1)
        return y;
    else if(x == 0)
        return 0;
    return func4(x - 1, y) + y + func4(x - 2, y);
}

int main()
{
    printf("%d\n", func4(9, 4));
    return 0;
}