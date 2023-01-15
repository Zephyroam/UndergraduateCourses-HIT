#include <stdio.h>
int fib_loop1(int n)
{
    int a = 1, b = 1;
    if(n == 0)
        return 0;
    for (int i = 3; i <= n; i++)
    {
        int tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}
long fib_loop2(int n)
{
    long a = 1, b = 1;
    if(n == 0)
        return 0;
    for (int i = 3; i <= n; i++)
    {
        long tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}
unsigned fib_loop3(int n)
{
    unsigned a = 1, b = 1;
    if(n == 0)
        return 0;
    for (int i = 3; i <= n; i++)
    {
        unsigned tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}
unsigned long fib_loop4(int n)
{
    unsigned long a = 1, b = 1;
    if(n == 0)
        return 0;
    for (int i = 3; i <= n; i++)
    {
        unsigned long tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}
int main()
{
    for (int i = 3;;i++)
    {
        if(fib_loop1(i) > fib_loop1(i + 1))
        {
            printf("%d\n", i + 1);
            break;
        }
    }
    for (int i = 3;;i++)
    {
        if(fib_loop2(i) > fib_loop2(i + 1))
        {
            printf("%d\n", i + 1);
            break;
        }
    }
    for (int i = 3;;i++)
    {
        if(fib_loop3(i) > fib_loop3(i + 1))
        {
            printf("%d\n", i + 1);
            break;
        }
    }
    for (int i = 3;;i++)
    {
        if(fib_loop4(i) > fib_loop4(i + 1))
        {
            printf("%d\n", i + 1);
            break;
        }
    }

    return 0;
}