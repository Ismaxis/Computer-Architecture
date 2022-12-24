
./resources/out.elf:     file format elf32-littleriscv

SYMBOL TABLE:
00010074 l    d  .text	00000000 .text
00010184 l    d  .eh_frame	00000000 .eh_frame
000111dc l    d  .bss	00000000 .bss
00000000 l    d  .comment	00000000 .comment
00000000 l    d  .riscv.attributes	00000000 .riscv.attributes
00000000 l    df *ABS*	00000000 test.cpp
000119dc g       *ABS*	00000000 __global_pointer$
000119ac g     O .bss	00000320 b
000111dc g       .eh_frame	00000000 __SDATA_BEGIN__
00010154 g     F .text	00000030 _Z9factoriali
00000000         *UND*	00000000 _start
000111dc g     O .bss	00000640 c
00011ccc g       .bss	00000000 __BSS_END__
000111dc g       .bss	00000000 __bss_start
00010074 g     F .text	00000040 main
000100d4 g     F .text	00000080 _Z4mmuli
000111dc g       .eh_frame	00000000 __DATA_BEGIN__
000111dc g       .eh_frame	00000000 _edata
00011ccc g       .bss	00000000 _end
0001181c g     O .bss	00000190 a



Disassembly of section .text:

00010074 <main>:
   10074:	ff010113          	addi	sp,sp,-16
   10078:	00112623          	sw	ra,12(sp)
   1007c:	00200713          	addi	a4,zero,2
   10080:	00100793          	addi	a5,zero,1
   10084:	00e50c63          	beq	a0,a4,1009c <main+0x28>
   10088:	00200693          	addi	a3,zero,2
   1008c:	00050713          	addi	a4,a0,0
   10090:	fff50513          	addi	a0,a0,-1
   10094:	02e787b3          	mul	a5,a5,a4
   10098:	fed51ae3          	bne	a0,a3,1008c <main+0x18>
   1009c:	00179513          	slli	a0,a5,0x1
   100a0:	034000ef          	jal	ra,100d4 <_Z4mmuli>
   100a4:	00c12083          	lw	ra,12(sp)
   100a8:	00000513          	addi	a0,zero,0
   100ac:	01010113          	addi	sp,sp,16
   100b0:	00008067          	jalr	zero,0(ra)
   100b4:	00000013          	addi	zero,zero,0
   100b8:	4025d513          	srai	a0,a1,0x2
   100bc:	00100137          	lui	sp,0x100
   100c0:	fb5ff0ef          	jal	ra,10074 <main>
   100c4:	00050593          	addi	a1,a0,0
   100c8:	00a00893          	addi	a7,zero,10
   100cc:	0ff0000f          	fence	iorw,iorw
   100d0:	00000073          	ecall

000100d4 <_Z4mmuli>:
   100d4:	00011fb7          	lui	t6,0x11
   100d8:	1dcf8793          	addi	a5,t6,476 # 111dc <c>
   100dc:	00050813          	addi	a6,a0,0
   100e0:	1dcf8f93          	addi	t6,t6,476
   100e4:	65478513          	addi	a0,a5,1620
   100e8:	fd018393          	addi	t2,gp,-48 # 119ac <b>
   100ec:	e4018293          	addi	t0,gp,-448 # 1181c <a>
   100f0:	02800f13          	addi	t5,zero,40
   100f4:	fec50e93          	addi	t4,a0,-20
   100f8:	000f8e13          	addi	t3,t6,0
   100fc:	00038313          	addi	t1,t2,0
   10100:	00000893          	addi	a7,zero,0
   10104:	00030693          	addi	a3,t1,0
   10108:	000e8713          	addi	a4,t4,0
   1010c:	00000613          	addi	a2,zero,0
   10110:	00070783          	lb	a5,0(a4)
   10114:	00069583          	lh	a1,0(a3)
   10118:	00170713          	addi	a4,a4,1
   1011c:	02868693          	addi	a3,a3,40
   10120:	02b787b3          	mul	a5,a5,a1
   10124:	010787b3          	add	a5,a5,a6
   10128:	00f60633          	add	a2,a2,a5
   1012c:	fee512e3          	bne	a0,a4,10110 <_Z4mmuli+0x3c>
   10130:	00ce2023          	sw	a2,0(t3)
   10134:	00288893          	addi	a7,a7,2
   10138:	004e0e13          	addi	t3,t3,4
   1013c:	00230313          	addi	t1,t1,2
   10140:	fde892e3          	bne	a7,t5,10104 <_Z4mmuli+0x30>
   10144:	050f8f93          	addi	t6,t6,80
   10148:	01450513          	addi	a0,a0,20
   1014c:	fa5f94e3          	bne	t6,t0,100f4 <_Z4mmuli+0x20>
   10150:	00008067          	jalr	zero,0(ra)

00010154 <_Z9factoriali>:
   10154:	00200713          	addi	a4,zero,2
   10158:	00100793          	addi	a5,zero,1
   1015c:	00200693          	addi	a3,zero,2
   10160:	00e50e63          	beq	a0,a4,1017c <_Z9factoriali+0x28>
   10164:	00050713          	addi	a4,a0,0
   10168:	fff50513          	addi	a0,a0,-1
   1016c:	02e787b3          	mul	a5,a5,a4
   10170:	fed51ae3          	bne	a0,a3,10164 <_Z9factoriali+0x10>
   10174:	00179513          	slli	a0,a5,0x1
   10178:	00008067          	jalr	zero,0(ra)
   1017c:	00200513          	addi	a0,zero,2
   10180:	00008067          	jalr	zero,0(ra)
