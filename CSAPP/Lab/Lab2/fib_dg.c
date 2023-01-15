#include <stdio.h>

int fib_dg1(int n)
{
    
    if(n == 2 || n == 1)
        return 1;
    else if(n == 0)
        return 0;
    return fib_dg1(n - 1) + fib_dg1(n - 2);
}
long fib_dg2(int n)
{
    if(n == 2 || n == 1)
        return 1;
    else if(n == 0)
        return 0;
    return fib_dg2(n - 1) + fib_dg2(n - 2);
}
unsigned fib_dg3(int n)
{
    if(n == 2 || n == 1)
        return 1;
    else if(n == 0)
        return 0;
    return fib_dg3(n - 1) + fib_dg3(n - 2);
}
unsigned long fib_dg4(int n)
{
    if(n == 2 || n == 1)
        return 1;
    else if(n == 0)
        return 0;
    return fib_dg4(n - 1) + fib_dg4(n - 2);
}
int main()
{
    for (int i = 3;;i++)
    {
        if(fib_dg1(i) > fib_dg1(i + 1))
        {
            printf("%d\n", i + 1);
            break;
        }
    }
    for (int i = 3;;i++)
    {
        if(fib_dg2(i) > fib_dg2(i + 1))
        {
            printf("%d\n", i + 1);
            break;
        }
    }
    for (int i = 3;;i++)
    {
        if(fib_dg3(i) > fib_dg3(i + 1))
        {
            printf("%d\n", i + 1);
            break;
        }
    }
    for (int i = 3;;i++)
    {
        if(fib_dg4(i) > fib_dg4(i + 1))
        {
            printf("%d\n", i + 1);
            break;
        }
    }

    return 0;
}