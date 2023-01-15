#include <stdio.h>
#include <string.h>

char cstr[100] = "1190200523-石翔宇";
char *pstr = "1190200523-石翔宇";

int main(int argc, char *argv[])
{
    strcpy(cstr, "131182200003105535");
    strcpy(pstr, "131182200003105535");
    printf("%s\n", cstr);
    printf("%s\n", pstr);
    return 0;
}
