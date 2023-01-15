#include <stdio.h>

int main()
{
    FILE *fp = fopen("hellowin.c", "r");
    char ch[4000];
    int p = 0;
    ch[p++] = fgetc(fp);
    while(ch[p - 1] != EOF)
    {
        ch[p++] = fgetc(fp);
    }
    for (int i = 0; i < p; i+=16)
    {
        for (int j = i; j < i + 16 && j < p; j++)
            if(ch[j] == '\n')
                printf("\\n ");
            else 
                printf("%c  ", ch[j]);
        puts("");
        for (int j = i; j < i + 16 && j < p; j++)
            printf("%x ", ch[j]);
        puts("");
    }
    return 0;
}