# functest3.s - An example of using C style functions
.section .data
precision:
   .byte 0x7f, 0x00
.section .bss
   .lcomm result, 4 #保留4个字节的空间，未设置初始值
.section .text
.globl _start
_start:
   nop
   finit
   fldcw precision

   pushl $10
   call area
   addl $4, %esp
   movl %eax, result

   pushl $2
   call area
   addl $4, %esp
   movl %eax, result

   pushl $120
   call area
   addl $4, %esp
   movl %eax, result

   movl $1, %eax
   movl $0, %ebx
   int $0x80

.type area, @function #声明 area为函数
area:                 #函数名字area的定义
   pushl %ebp         #保存ebp
   movl %esp, %ebp    #在ebp中保存esp的数值，后面作为局部变量、参数寻址的基址使用
   subl $4, %esp      #保留4字节的空间，地址是 ebp-4
   fldpi              #3.141592压入fpu栈，即st(0)
   filds 8(%ebp)      #将 栈中ebp+8处的单精度数值（32位的半径r)压入fpu栈，即st(0)值为半径,st(1)数值为pi
   fmul %st(0), %st(0)#计算st(0)*st(0)， 结果保存在st(0)
   fmulp %st(0), %st(1)#st(1)数值为pi，计算st(0)*st(1),结果为面值数值存在st(1),并将原st(0)出栈，最终st(0)=面积值 
   fstps -4(%ebp)      #将st(0)（计算好的面积数值）拷贝到（ebp-4）
   movl -4(%ebp), %eax #将（ebp-4）中保存的面积数值拷贝到eax （不能从st(0)直接向通用寄存器拷贝，需要用内存中转一下）

   movl %ebp, %esp  #恢复栈到刚刚进入函数时的状态
   popl %ebp
   ret              #函数返回
