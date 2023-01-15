
/*
A. 不正确。当REG为%rsp时，pushq %rsp的行为应该是将原始的%rsp值放入栈顶；而这段代码将%rsp-8的值放入了栈顶。
B. 
    movq REG, -8(%rsp)
    subq $8, %rsp
*/