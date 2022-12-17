
.\resources\out.elf:     file format elf32-littleriscv

SYMBOL TABLE:
00010074 l    d  .text	00000000 .text
00010128 l    d  .eh_frame	00000000 .eh_frame
0001116c l    d  .bss	00000000 .bss
00000000 l    d  .comment	00000000 .comment
00000000 l    d  .riscv.attributes	00000000 .riscv.attributes
00000000 l    df *ABS*	00000000 test.cpp
0001196c g       *ABS*	00000000 __global_pointer$
0001193c g     O .bss	00000320 b
0001116c g       .eh_frame	00000000 __SDATA_BEGIN__
00000000         *UND*	00000000 _start
0001116c g     O .bss	00000640 c
00011c5c g       .bss	00000000 __BSS_END__
0001116c g       .bss	00000000 __bss_start
00010074 g     F .text	0000001c main
000100b0 g     F .text	00000078 _Z4mmulv
0001116c g       .eh_frame	00000000 __DATA_BEGIN__
0001116c g       .eh_frame	00000000 _edata
00011c5c g       .bss	00000000 _end
000117ac g     O .bss	00000190 a



Disassembly of section .text:

00010074 <main>:
   10074:	ff010113          	addi	sp,sp,-16
   10078:	00112623          	sw	ra,12(sp)
   1007c:	034000ef          	jal	ra,100b0 <_Z4mmulv>
   10080:	00c12083          	lw	ra,12(sp)
   10084:	00000513          	addi	a0,zero,0
   10088:	01010113          	addi	sp,sp,16
   1008c:	00008067          	jalr	zero,0(ra)
   10090:	00000013          	addi	zero,zero,0
   10094:	4025d513          	srai	a0,a1,0x2
   10098:	00100137          	lui	sp,0x100
   1009c:	fd9ff0ef          	jal	ra,10074 <main>
   100a0:	00050593          	addi	a1,a0,0
   100a4:	00a00893          	addi	a7,zero,10
   100a8:	0ff0000f          	fence	iorw,iorw
   100ac:	00000073          	ecall

000100b0 <_Z4mmulv>:
   100b0:	00011f37          	lui	t5,0x11
   100b4:	16cf0513          	addi	a0,t5,364 # 1116c <c>
   100b8:	65450513          	addi	a0,a0,1620
   100bc:	16cf0f13          	addi	t5,t5,364
   100c0:	fd018293          	addi	t0,gp,-48 # 1193c <b>
   100c4:	e4018f93          	addi	t6,gp,-448 # 117ac <a>
   100c8:	02800e93          	addi	t4,zero,40
   100cc:	fec50e13          	addi	t3,a0,-20
   100d0:	000f0313          	addi	t1,t5,0
   100d4:	00028893          	addi	a7,t0,0
   100d8:	00000813          	addi	a6,zero,0
   100dc:	00088693          	addi	a3,a7,0
   100e0:	000e0793          	addi	a5,t3,0
   100e4:	00000613          	addi	a2,zero,0
   100e8:	00078703          	lb	a4,0(a5)
   100ec:	00069583          	lh	a1,0(a3)
   100f0:	00178793          	addi	a5,a5,1
   100f4:	02868693          	addi	a3,a3,40
   100f8:	02b70733          	mul	a4,a4,a1
   100fc:	00e60633          	add	a2,a2,a4
   10100:	fea794e3          	bne	a5,a0,100e8 <_Z4mmulv+0x38>
   10104:	00c32023          	sw	a2,0(t1)
   10108:	00280813          	addi	a6,a6,2
   1010c:	00430313          	addi	t1,t1,4
   10110:	00288893          	addi	a7,a7,2
   10114:	fdd814e3          	bne	a6,t4,100dc <_Z4mmulv+0x2c>
   10118:	050f0f13          	addi	t5,t5,80
   1011c:	01478513          	addi	a0,a5,20
   10120:	fbff16e3          	bne	t5,t6,100cc <_Z4mmulv+0x1c>
   10124:	00008067          	jalr	zero,0(ra)
