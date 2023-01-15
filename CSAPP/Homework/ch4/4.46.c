
/*
A. 不正确。当REG为%rsp时，popq %rsp的行为应该是将栈顶的值给%rsp；而这段代码运行后%rsp的值为栈顶值-8。
B. 
    addq $8, %rsp
    movq -8(%rsp), REG
*/