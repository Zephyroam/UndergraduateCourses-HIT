
hello.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <main>:
   0:	f3 0f 1e fa          	endbr64 
   4:	55                   	push   %rbp
   5:	48 89 e5             	mov    %rsp,%rbp
   8:	48 83 ec 20          	sub    $0x20,%rsp
   c:	89 7d ec             	mov    %edi,-0x14(%rbp)
   f:	48 89 75 e0          	mov    %rsi,-0x20(%rbp)
  13:	83 7d ec 03          	cmpl   $0x3,-0x14(%rbp)
  17:	74 16                	je     2f <main+0x2f>
  19:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 20 <main+0x20>
			1c: R_X86_64_PC32	.rodata-0x4
  20:	e8 00 00 00 00       	callq  25 <main+0x25>
			21: R_X86_64_PLT32	puts-0x4
  25:	bf 01 00 00 00       	mov    $0x1,%edi
  2a:	e8 00 00 00 00       	callq  2f <main+0x2f>
			2b: R_X86_64_PLT32	exit-0x4
  2f:	c7 45 fc 00 00 00 00 	movl   $0x0,-0x4(%rbp)
  36:	eb 3b                	jmp    73 <main+0x73>
  38:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  3c:	48 83 c0 10          	add    $0x10,%rax
  40:	48 8b 10             	mov    (%rax),%rdx
  43:	48 8b 45 e0          	mov    -0x20(%rbp),%rax
  47:	48 83 c0 08          	add    $0x8,%rax
  4b:	48 8b 00             	mov    (%rax),%rax
  4e:	48 89 c6             	mov    %rax,%rsi
  51:	48 8d 3d 00 00 00 00 	lea    0x0(%rip),%rdi        # 58 <main+0x58>
			54: R_X86_64_PC32	.rodata+0x1a
  58:	b8 00 00 00 00       	mov    $0x0,%eax
  5d:	e8 00 00 00 00       	callq  62 <main+0x62>
			5e: R_X86_64_PLT32	printf-0x4
  62:	8b 05 00 00 00 00    	mov    0x0(%rip),%eax        # 68 <main+0x68>
			64: R_X86_64_PC32	sleepsecs-0x4
  68:	89 c7                	mov    %eax,%edi
  6a:	e8 00 00 00 00       	callq  6f <main+0x6f>
			6b: R_X86_64_PLT32	sleep-0x4
  6f:	83 45 fc 01          	addl   $0x1,-0x4(%rbp)
  73:	83 7d fc 09          	cmpl   $0x9,-0x4(%rbp)
  77:	7e bf                	jle    38 <main+0x38>
  79:	e8 00 00 00 00       	callq  7e <main+0x7e>
			7a: R_X86_64_PLT32	getchar-0x4
  7e:	b8 00 00 00 00       	mov    $0x0,%eax
  83:	c9                   	leaveq 
  84:	c3                   	retq   
