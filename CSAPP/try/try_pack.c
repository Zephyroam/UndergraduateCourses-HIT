#include <stdlib.h>

struct S1 {
    char c;
    int i[2];
    double v;
}S1;

#pragma pack(push)
#pragma pack(1)
struct S1_pack1 {
    char c;
    int i[2];
    double v;
}S1_pack1;
#pragma pack(pop)

int main()
{
    printf("sizeof S1 = %u\n",sizeof(S1));
    printf("sizeof S1_pack1 = %u\n",sizeof(S1_pack1));
    return 0;
}
    
