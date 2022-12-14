
.\out.elf:     file format elf32-littleriscv

SYMBOL TABLE:
00010054 l    d  .text	00000000 .text
000100b4 l    d  .eh_frame	00000000 .eh_frame
00000000 l    d  .comment	00000000 .comment
00000000 l    d  .riscv.attributes	00000000 .riscv.attributes
00000000 l    df *ABS*	00000000 test.cpp
000118f0 g       *ABS*	00000000 __global_pointer$
000110f0 g       .eh_frame	00000000 __SDATA_BEGIN__
00010084 g     F .text	00000030 _Z9factoriali
00000000         *UND*	00000000 _start
000110f0 g       .eh_frame	00000000 __BSS_END__
000110f0 g       .eh_frame	00000000 __bss_start
00010054 g     F .text	00000030 main
000110f0 g       .eh_frame	00000000 __DATA_BEGIN__
000110f0 g       .eh_frame	00000000 _edata
000110f0 g       .eh_frame	00000000 _end



Disassembly of section .text:

00010054 <main>:
   10054:	00200693          	addi	a3,zero,2
   10058:	00050793          	addi	a5,a0,0
   1005c:	00100713          	addi	a4,zero,1
   10060:	00200613          	addi	a2,zero,2
   10064:	00d50a63          	beq	a0,a3,10078 <main+0x24>
   10068:	00078693          	addi	a3,a5,0
   1006c:	fff78793          	addi	a5,a5,-1
   10070:	02d70733          	mul	a4,a4,a3
   10074:	fec79ae3          	bne	a5,a2,10068 <main+0x14>
   10078:	00171713          	slli	a4,a4,0x1
   1007c:	00e52533          	slt	a0,a0,a4
   10080:	00008067          	jalr	zero,0(ra)

00010084 <_Z9factoriali>:
   10084:	00200713          	addi	a4,zero,2
   10088:	00100793          	addi	a5,zero,1
   1008c:	00200693          	addi	a3,zero,2
   10090:	00e50e63          	beq	a0,a4,100ac <_Z9factoriali+0x28>
   10094:	00050713          	addi	a4,a0,0
   10098:	fff50513          	addi	a0,a0,-1
   1009c:	02e787b3          	mul	a5,a5,a4
   100a0:	fed51ae3          	bne	a0,a3,10094 <_Z9factoriali+0x10>
   100a4:	00179513          	slli	a0,a5,0x1
   100a8:	00008067          	jalr	zero,0(ra)
   100ac:	00200513          	addi	a0,zero,2
   100b0:	00008067          	jalr	zero,0(ra)
