#include <stdio.h>

void explode();
int phase_6(int x, int y)
{
    int a_40[6];
    scanf("%d%d%d%d%d%d", a_40, a_40 + 1, a_40 + 2, a_40 + 3, a_40 + 4, a_40 + 5);
    int r12d = 0;
    int rax;
    int ebx;
    
    while (r12d <= 5)
    {
        rax = *(a_40 + r12d);
        rax--;
        if(rax > 5)
            explode();
        ebx = r12d + 1;
        while(ebx <= 5)
        {
            rax = r12d;
            if(*(a_40 + ebx) == *(a_40 + rax))
                explode();
            ebx++;
        }
    }//输入之间不能相等

    int *rdx;
    int a_70[6];
    for (int esi = 0; esi <= 5; esi++)
    {
        rax = 1;
        rdx = 0x4052d0;
        while(a_40[esi] > rax)
        {
            rdx = *(rdx + 1);
            rax++;
        }
        a_70[esi] = rdx;
    }//

    int *rcx = a_70[0];
    rax = 1;
    while(rax <= 5)
    {
        rdx = a_70[rax];
        *(rcx + 1) = rdx;
        rax++;
        rcx = rdx;
    }//形成链表

    int *rbx = a_70[0];
    r12d = 0;
    while(r12d <= 4)
    {
        if(*rbx > *(*(rbx + 1)))
            explode();
        else
        {
            rbx = *(rbx + 1);
            r12d++;
        }
    }//链表要递增


    


}

int main()
{
    
    return 0;
}