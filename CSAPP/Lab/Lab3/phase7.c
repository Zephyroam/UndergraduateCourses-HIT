#include <stdio.h>

int esi = 1;
int fun7(int *edi)
{
    if(*edi > esi)
    {
        edi = *(edi + 1);
        return fun7(edi) * 2;
    }
    else if(*edi != esi)
    {
        edi = *(edi + 2);
        return fun7(edi) * 2 + 1;
    }
    return 0;
}

int main()
{
    freopen("phase7.txt", "r", stdin);
    char a[2];
    while(scanf("%s", a) != EOF)
    {
        int x = 0;
        if(a[0] >= '0' && a[0] <= '9')
            x = a[0] - '0';
        else if(a[0] >= 'a' && a[0] <= 'f')
            x = a[0] - 'a' + 10;
        x = x * 16;
        if(a[1] >= '0' && a[1] <= '9')
            x += a[1] - '0';
        else if(a[1] >= 'a' && a[1] <= 'f')
            x += a[1] - 'a' + 10;
        printf("%c", x);
    }
    return 0;
}