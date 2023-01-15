#include <stdio.h>
/*
2.59题：
*/
unsigned _2_59()
{
    unsigned x = 0x89abcdef, y = 0x76543210;
    return x & 0xff | (y & ~0xff);
}

/*
2.63题：
*/
unsigned srl(unsigned x, int k)
{
    unsigned xsra = (int)x >> k;
    unsigned y = (~((x & 0x80000000) && 1)) + 1;
    return xsra ^ (y << (32 - k)) ;
}
int sra(int x, int k)
{
    int xsrl = (unsigned)x >> k;
    unsigned y = (~((x & 0x80000000) && 1)) + 1;
    return xsrl ^ (y << (32 - k));
}

/*
2.67题：
A.
左移运算左移的范围是0~31
B.
如下
C.
如下
*/
int int_size_is_32_B()
{
    int set_msb = 1 << 31;
    int beyond_msb = set_msb << 1;
    return set_msb && !beyond_msb;
}
int int_size_is_32_C()
{
    int set_msb = 1 << 15 << 15 << 1;
    int beyond_msb = set_msb << 1;
    return set_msb && !beyond_msb;
}

/*
2.71题：
A.
无法处理word为负数的情况
B.
如下
*/
typedef unsigned packed_t;
int xbyte(packed_t word, int bytenum)
{
    int x = word >> (bytenum << 3);
    int y = (x - (x >> 8 << 8)) << 24 >> 24;
    return y;
}

/*
2.75题：
*/
int signed_high_prod(int x, int y)
{
    long long s = (long long)x * y;
    return s >> 32;
}
unsigned unsigned_high_prod(unsigned x, unsigned y)
{
    int m = signed_high_prod((int)x, (int)y);
    return m + x * (y >> 31) + y * (x >> 31);
}

/*
2.79题：
*/
int mul3div4(int x)
{
    return (x >> 1) + (x >> 2) + ((x & 0x3) == 0x3);
}

/*
2.83题：
A.
n = 0.yyyyyy
n << k = y.yyyyyyy = Y + n
n << k - n = Y
n * (2^k - 1) = Y
n = Y / (2^k - 1)

B.
(a) 5/7
(b) 2/5
(c) 19/63
*/

/*
2.87题：
8000 0 -14 -0 -0.0
4001 1025/1024 1 1025*2^{-9} 2.00195312
6000 1 9 512 512.0
03ff 1023/1024 -14 1023*2^{-24} 6.09755516e-5
fc00
3bb0 123/64 -1 123/128 0.9609375
*/

/*
2.91题：
A. 
11.0010010000111111011011

B. 
11.001001001001[001]

C. 
9
*/

/*
2.95题：
*/
typedef unsigned float_bits;
float_bits float_half(float_bits f)
{
    float_bits exp = (f >> 23) & 0xff;
    float_bits frac = f & 0x7fffff;
    float_bits ou = (exp & 0x3) == 0x3;
    if (exp == 0)
    {
        frac >>= 1;
        frac += ou;
    }
    else if (exp == 1)
    {
        frac >>= 1;
        frac += ou + (1 << 22);
    }
    else if (exp != 255)
        exp--;
    return (f & 0x80000000) | (exp << 23) | frac;
}