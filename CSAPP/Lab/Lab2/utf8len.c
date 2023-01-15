#include <stdio.h>
#include <string.h>

int utf8len(char* str)
{
    int cnt = 0;
    while(*str != '\0')
    {
        unsigned char now = *str;
        if((now >> 7) == 0)
            str++;
        else if((now >> 5) == 6)
            str += 2;
        else if((now >> 4) == 14)
            str += 3;
        else if((now >> 3) == 30)
            str += 4;
        cnt++;
    }
    return cnt;
}

int main()
{
    char a[] = "123石翔宇123456789";
    printf("%d\n", utf8len(a));
    char b[] = "石";
    char c[] = "宇";
    char d[] = "翔";
    printf("b: %x %x %x\n", *b, *(b + 1), *(b + 2));
    printf("c: %x %x %x\n", *c, *(c + 1), *(c + 2));
    printf("d: %x %x %x\n", *d, *(d + 1), *(d + 2));
    printf("%d", strcmp(b, d));
    return 0;
}
