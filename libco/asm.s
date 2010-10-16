/* Copyright (c) 2005-2006 Russ Cox, MIT; see COPYRIGHT */


.section ".toc","aw"
.tc32 _getmcontext[TC],_getmcontext
.tc32 _setmcontext[TC],_setmcontext

.section .text
   .globl _getmcontext
   .section ".opd","aw"
   .align 2
   _getmcontext:
      .long ._getmcontext,.TOC.@tocbase32
      .size _getmcontext,.-_getmcontext
      .previous
      .type ._getmcontext,@function
      .globl ._getmcontext

   .section .text
   .globl _setmcontext
   .section ".opd","aw"
   .align 2
   _setmcontext:
      .long ._setmcontext,.TOC.@tocbase32
      .size _setmcontext,.-_setmcontext
      .previous
      .type ._setmcontext,@function
      .globl ._setmcontext

._getmcontext:				/* xxx: instruction scheduling */
	mflr	0
	mfcr	5
	mfctr	6
	mfxer	7
	std	0, 0(3)        /* PC, 4 bytes on PS3 */
	std	5, 1*8(3)
	std	6, 2*8(3)
	std	7, 3*8(3)

	std	1, 4*8(3)       /* SP, 4 bytes on PS3 */
	std	2, 5*8(3)
	li	   5, 1			/* return value for setmcontext */
	std	5, 6*8(3)

	std	13, (0+7)*8(3)	/* callee-save GPRs */
	std	14, (1+7)*8(3)	/* xxx: block move */
	std	15, (2+7)*8(3)
	std	16, (3+7)*8(3)
	std	17, (4+7)*8(3)
	std	18, (5+7)*8(3)
	std	19, (6+7)*8(3)
	std	20, (7+7)*8(3)
	std	21, (8+7)*8(3)
	std	22, (9+7)*8(3)
	std	23, (10+7)*8(3)
	std	24, (11+7)*8(3)
	std	25, (12+7)*8(3)
	std	26, (13+7)*8(3)
	std	27, (14+7)*8(3)
	std	28, (15+7)*8(3)
	std	29, (16+7)*8(3)
	std	30, (17+7)*8(3)
	std	31, (18+7)*8(3)

	li	   3, 0			/* return */
	blr


._setmcontext:
	ld	13, (0+7)*8(3)	/* callee-save GPRs */
	ld	14, (1+7)*8(3)	/* xxx: block move */
	ld	15, (2+7)*8(3)
	ld	16, (3+7)*8(3)
	ld	17, (4+7)*8(3)
	ld	18, (5+7)*8(3)
	ld	19, (6+7)*8(3)
	ld	20, (7+7)*8(3)
	ld	21, (8+7)*8(3)
	ld	22, (9+7)*8(3)
	ld	23, (10+7)*8(3)
	ld	24, (11+7)*8(3)
	ld	25, (12+7)*8(3)
	ld	26, (13+7)*8(3)
	ld	27, (14+7)*8(3)
	ld	28, (15+7)*8(3)
	ld	29, (16+7)*8(3)
	ld	30, (17+7)*8(3)
	ld	31, (18+7)*8(3)

	ld	1, 4*8(3)   /* SP, 4 bytes on PS3 */
	ld	2, 5*8(3)

	ld	0, 0(3)    /* PC, 4 bytes on PS3. Big endian, so we use the low bytes only */
	mtlr	0
	ld	0, 1*8(3)
	mtcr	0			/* mtcrf 0xFF, r0 */
	ld	0, 2*8(3)
	mtctr	0
	ld	0, 3*8(3)
	mtxer	0

	ld	3,	6*8(3)
	blr

