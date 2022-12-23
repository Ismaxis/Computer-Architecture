.text
00010074   <main>:
   10074:	ff010113	addi	sp, sp, -16
   10078:	00112623	sw	ra, 12(sp)
   1007c:	034000ef	jal	ra, 000100b0
   10080:	00c12083	lw	ra, 12(sp)
   10084:	00000513	addi	a0, zero, 0
   10088:	01010113	addi	sp, sp, 16
   1008c:	00008067	jalr	zero, 0(ra)
   10090:	00000013	addi	zero, zero, 0
   10094:	4025d513	srli	a0, a1, 1026
   10098:	00100137	lui	sp, 1048576
   1009c:	fd9ff0ef	jal	ra, 00010074
   100a0:	00050593	addi	a1, a0, 0
   100a4:	00a00893	addi	a7, zero, 10
   100a8:	0ff0000f	unknown_instruction
   100ac:	00000073	ecall		
000100b0   <_Z4mmulv>:
   100b0:	00011f37	lui	t5, 69632
   100b4:	1b0f0513	addi	a0, t5, 432
   100b8:	65450513	addi	a0, a0, 1620
   100bc:	1b0f0f13	addi	t5, t5, 432
   100c0:	fd018293	addi	t0, gp, -48
   100c4:	e4018f93	addi	t6, gp, -448
   100c8:	02800e93	addi	t4, zero, 40
   100cc:	fec50e13	addi	t3, a0, -20
   100d0:	000f0313	addi	t1, t5, 0
   100d4:	00028893	addi	a7, t0, 0
   100d8:	00000813	addi	a6, zero, 0
   100dc:	00088693	addi	a3, a7, 0
   100e0:	000e0793	addi	a5, t3, 0
   100e4:	00000613	addi	a2, zero, 0
   100e8:	00078703	lb	a4, 0(a5)
   100ec:	00069583	lh	a1, 0(a3)
   100f0:	00178793	addi	a5, a5, 1
   100f4:	02868693	addi	a3, a3, 40
   100f8:	02b70733	mul	a4, a4, a1
   100fc:	00e60633	add	a2, a2, a4
   10100:	fea794e3	bne	a5, a0, 000100e8
   10104:	00c32023	sw	a2, 0(t1)
   10108:	00280813	addi	a6, a6, 2
   1010c:	00430313	addi	t1, t1, 4
   10110:	00288893	addi	a7, a7, 2
   10114:	fdd814e3	bne	a6, t4, 000100dc
   10118:	050f0f13	addi	t5, t5, 80
   1011c:	01478513	addi	a0, a5, 20
   10120:	fbff16e3	bne	t5, t6, 000100cc
   10124:	00008067	jalr	zero, 0(ra)
00010128   <_Z9factoriali>:
   10128:	00200713	addi	a4, zero, 2
   1012c:	00100793	addi	a5, zero, 1
   10130:	00200693	addi	a3, zero, 2
   10134:	00e50e63	beq	a0, a4, 00010150
   10138:	00050713	addi	a4, a0, 0
   1013c:	fff50513	addi	a0, a0, -1
   10140:	02e787b3	mul	a5, a5, a4
   10144:	fed51ae3	bne	a0, a3, 00010138
   10148:	00179513	slli	a0, a5, 1
   1014c:	00008067	jalr	zero, 0(ra)
   10150:	00200513	addi	a0, zero, 2
   10154:	00008067	jalr	zero, 0(ra)

.symtab
Symbol Value          	  Size Type 	 Bind 	 Vis   	  Index Name
[   0] 0x0                   0 NOTYPE   LOCAL    DEFAULT   UNDEF 
[   1] 0x10074               0 SECTION  LOCAL    DEFAULT       1 
[   2] 0x10158               0 SECTION  LOCAL    DEFAULT       2 
[   3] 0x111B0               0 SECTION  LOCAL    DEFAULT       3 
[   4] 0x0                   0 SECTION  LOCAL    DEFAULT       4 
[   5] 0x0                   0 SECTION  LOCAL    DEFAULT       5 
[   6] 0x0                   0 FILE     LOCAL    DEFAULT     ABS test.cpp
[   7] 0x119B0               0 NOTYPE   GLOBAL   DEFAULT     ABS __global_pointer$
[   8] 0x11980             800 OBJECT   GLOBAL   DEFAULT       3 b
[   9] 0x111B0               0 NOTYPE   GLOBAL   DEFAULT       2 __SDATA_BEGIN__
[  10] 0x10128              48 FUNC     GLOBAL   DEFAULT       1 _Z9factoriali
[  11] 0x0                   0 NOTYPE   GLOBAL   DEFAULT   UNDEF _start
[  12] 0x111B0            1600 OBJECT   GLOBAL   DEFAULT       3 c
[  13] 0x11CA0               0 NOTYPE   GLOBAL   DEFAULT       3 __BSS_END__
[  14] 0x111B0               0 NOTYPE   GLOBAL   DEFAULT       3 __bss_start
[  15] 0x10074              28 FUNC     GLOBAL   DEFAULT       1 main
[  16] 0x100B0             120 FUNC     GLOBAL   DEFAULT       1 _Z4mmulv
[  17] 0x111B0               0 NOTYPE   GLOBAL   DEFAULT       2 __DATA_BEGIN__
[  18] 0x111B0               0 NOTYPE   GLOBAL   DEFAULT       2 _edata
[  19] 0x11CA0               0 NOTYPE   GLOBAL   DEFAULT       3 _end
[  20] 0x117F0             400 OBJECT   GLOBAL   DEFAULT       3 a
