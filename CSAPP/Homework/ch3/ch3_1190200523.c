#include <stdio.h>
/*
3.59题：
store_prod:
  movq %rdx, %rax     # %rax = y
  cqto                # %rdx = -y_63
  movq %rsi, %rcx     # %rcx = x
  sarq $63, %rcx      # %rcx = -x_63
  imulq %rax, %rcx    # %rcx = y * -x_63
  imulq %rsi, %rdx    # %rdx = x * -y_63
  addq %rdx, %rcx     # %rcx = x * -y_63 + y * -x_63
  mulq %rsi           # %rdx:%rax <= ux * uy
  addq %rcx, %rdx     # %rdx = ux * uy(高64位) - (x_{63}y + y_{63}x)2^{64}
  movq %rax, (%rdi)   # 设置低64位
  movq %rdx, 8(%rdi)  # 设置高64位
  ret
*/


/*
3.61题：
*/
long cread_alt(long *xp)
{
    return (!xp ? 0 : *xp);
}


/*
3.63题：
*/
long switch_prob(long x, long n)
{
    long result = x;
    switch(n)
    {
        case 60:
        case 62:
            result = x * 8;
            break;
        case 63:
            result = x >> 3;
            break;
        case 64:
            result = x * 15;
            x = result;
        case 65:
            x *= x;
        default:
            result = x + 0x4b;
    }
    return result;
}


/*
3.65题：
A. %rdx
B. %rax
C. 15
*/


/*
3.67题：
A.
104  +------------------+
     |                  |
     |                  |
     |                  |
     |                  |
     |                  |
     |                  |
     |                  |
     |                  |
 64  +------------------+ <-- %rdi
     |                  |
     |                  |
     |                  |
     |                  |
     |                  |
     |                  |
 32  +------------------+
     |         z        |
 24  +------------------+
     |        &z        |
 16  +------------------+
     |         y        |
  8  +------------------+
     |         x        |
  0  +------------------+ <-- %rsp

B. 地址%rsp+64
C. 通过%rsp+偏移量来访问
D. 设置到%rdi+偏移量,即%rsp+64+偏移量
E. 
104  +------------------+
     |                  |
     |                  |
     |                  |
     |                  |
     |                  |
 88  +------------------+
     |         z        |
 80  +------------------+
     |         x        |
 72  +------------------+
     |         y        |
 64  +------------------+ <-- %rdi
     |                  |
     |                  |
     |                  |
     |                  |
     |                  |
     |                  |
 32  +------------------+
     |         z        |
 24  +------------------+
     |        &z        |
 16  +------------------+
     |         y        |
  8  +------------------+
     |         x        |
  0  +------------------+ <-- %rsp

F. 实际传递的会是一个空的位置指针，函数把数据存储在这个位置上，同时返回值也是这个指针。
*/


/*
3.69题：
A. 7
B. 如下
*/
struct a_struct
{
    long idx;
    long x[4];
};



/*
3.71题：
*/
#define MAX_SIZE 1000
void good_echo()
{
    char a[MAX_SIZE];
    while(1)
    {
        char *now = fgets(a, MAX_SIZE, stdin);
        if (now == NULL)
            break;
        printf("%s", a);
    }
}