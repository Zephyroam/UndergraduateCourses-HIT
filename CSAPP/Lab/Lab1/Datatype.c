#include <stdio.h>

char ch = 'a';
int i = 1190200523;
unsigned int ui = 1190200523;
long l = 1190200523;
float f = 1190200523;
double d = 1190200523;
int *ip = &i;
int ay[3] = {0xff11, 0xff12, 0xff13};
struct node
{
    int x;
}Node;

union Token
{
   char cval;
   int ival;
}un;
enum color_set
{
    RED,
    BLUE
}em;

typedef unsigned char *byte_pointer;
void show_bytes(byte_pointer start,size_t len)
{
    printf("\tHex:");
    for (size_t i = 0; i < len; i++)
        printf(" %.2x", start[i]);
    printf("\n");
}

int main()
{
    Node.x = 0xabcd;
    un.cval = 'c';
    em = BLUE;

    puts("char:");
    printf("\tName: ch\n");
    printf("\tContent: %c\n", ch);
    printf("\tAddr: %p\n", &ch);
    show_bytes((byte_pointer)&ch, sizeof(char));

    puts("int:");
    printf("\tName: i\n");
    printf("\tContent: %d\n", i);
    printf("\tAddr: %p\n", &i);
    show_bytes((byte_pointer)&i, sizeof(int));

    puts("unsigned int:");
    printf("\tName: ui\n");
    printf("\tContent: %u\n", ui);
    printf("\tAddr: %p\n", &ui);
    show_bytes((byte_pointer)&ui, sizeof(unsigned int));

    puts("long:");
    printf("\tName: l\n");
    printf("\tContent: %ld\n", l);
    printf("\tAddr: %p\n", &l);
    show_bytes((byte_pointer)&l, sizeof(long));

    puts("float:");
    printf("\tName: f\n");
    printf("\tContent: %f\n", f);
    printf("\tAddr: %p\n", &f);
    show_bytes((byte_pointer)&f, sizeof(float));

    puts("double:");
    printf("\tName: d\n");
    printf("\tContent: %lf\n", d);
    printf("\tAddr: %p\n", &d);
    show_bytes((byte_pointer)&d, sizeof(double));

    puts("pointer:");
    printf("\tName: ip\n");
    printf("\tContent: %p\n", ip);
    printf("\tAddr: %p\n", &ip);
    show_bytes((byte_pointer)&ip, sizeof(int *));

    puts("array:");
    printf("\tName: ay[0], ay[1], ay[2]\n");
    printf("\tContent: %d\t%d\t%d\n", ay[0], ay[1], ay[2]);
    printf("\tAddr: %p\t%p\t%p\n", &ay[0], &ay[1], &ay[2]);
    show_bytes((byte_pointer)&ay[0], sizeof(int));
    show_bytes((byte_pointer)&ay[1], sizeof(int));
    show_bytes((byte_pointer)&ay[2], sizeof(int));

    puts("struct:");
    printf("\tName: Node\n");
    printf("\tContent: %d\n", Node.x);
    printf("\tAddr: %p\n", &Node.x);
    show_bytes((byte_pointer)&Node.x, sizeof(int));

    puts("union:");
    printf("\tName: un\n");
    printf("\tContent: %c\t%i\n", un.cval, un.ival);
    printf("\tAddr: %p\t%p\n",&un.cval, &un.ival);
    show_bytes((byte_pointer)&un.cval, sizeof(char));
    show_bytes((byte_pointer)&un.ival, sizeof(int));

    puts("enum:");
    printf("\tName: em\n");
    printf("\tContent: %i\n", em);
    printf("\tAddr: %p\n", &em);
    show_bytes((byte_pointer)&em, sizeof(int));

    puts("");

    printf("Addr of fuc main: %p\n", main);
    printf("Addr of fuc printf: %p\n", printf);
    return 0;
}