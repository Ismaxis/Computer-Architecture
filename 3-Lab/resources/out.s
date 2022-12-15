
.\resources\out.elf:     file format elf32-littleriscv

SYMBOL TABLE:
00010054 l    d  .text	00000000 .text
000100d8 l    d  .eh_frame	00000000 .eh_frame
00000000 l    d  .comment	00000000 .comment
00000000 l    d  .riscv.attributes	00000000 .riscv.attributes
00000000 l    df *ABS*	00000000 test.cpp
00011914 g       *ABS*	00000000 __global_pointer$
00011114 g       .eh_frame	00000000 __SDATA_BEGIN__
000100a8 g     F .text	00000030 _Z9factoriali
00000000         *UND*	00000000 _start
00011114 g       .eh_frame	00000000 __BSS_END__
00011114 g       .eh_frame	00000000 __bss_start
00010054 g     F .text	00000054 main
00011114 g       .eh_frame	00000000 __DATA_BEGIN__
00011114 g       .eh_frame	00000000 _edata
00011114 g       .eh_frame	00000000 _end



Disassembly of section .text:

00010054 <main>:
   10054:	00200793          	addi	a5,zero,2
   10058:	00050593          	addi	a1,a0,0
   1005c:	04f50263          	beq	a0,a5,100a0 <main+0x4c>
   10060:	00050793          	addi	a5,a0,0
   10064:	00100713          	addi	a4,zero,1
   10068:	00200613          	addi	a2,zero,2
   1006c:	00078693          	addi	a3,a5,0
   10070:	fff78793          	addi	a5,a5,-1
   10074:	02d70733          	mul	a4,a4,a3
   10078:	fec79ae3          	bne	a5,a2,1006c <main+0x18>
   1007c:	00171713          	slli	a4,a4,0x1
   10080:	00100793          	addi	a5,zero,1
   10084:	00b75733          	srl	a4,a4,a1
   10088:	00b797b3          	sll	a5,a5,a1
   1008c:	00e78733          	add	a4,a5,a4
   10090:	00100513          	addi	a0,zero,1
   10094:	00e5c863          	blt	a1,a4,100a4 <main+0x50>
   10098:	00e58533          	add	a0,a1,a4
   1009c:	00008067          	jalr	zero,0(ra)
   100a0:	00100513          	addi	a0,zero,1
   100a4:	00008067          	jalr	zero,0(ra)

000100a8 <_Z9factoriali>:
   100a8:	00200713          	addi	a4,zero,2
   100ac:	00100793          	addi	a5,zero,1
   100b0:	00200693          	addi	a3,zero,2
   100b4:	00e50e63          	beq	a0,a4,100d0 <_Z9factoriali+0x28>
   100b8:	00050713          	addi	a4,a0,0
   100bc:	fff50513          	addi	a0,a0,-1
   100c0:	02e787b3          	mul	a5,a5,a4
   100c4:	fed51ae3          	bne	a0,a3,100b8 <_Z9factoriali+0x10>
   100c8:	00179513          	slli	a0,a5,0x1
   100cc:	00008067          	jalr	zero,0(ra)
   100d0:	00200513          	addi	a0,zero,2
   100d4:	00008067          	jalr	zero,0(ra)
