
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
	stw	0, 0*4(3)
	stw	5, 1*4(3)
	stw	6, 2*4(3)
	stw	7, 3*4(3)

	stw	1, 4*4(3)
	stw	2, 5*4(3)
	li	5, 1			/* return value for setmcontext */
	stw	5, 6*4(3)

	stw	13, (0+7)*4(3)	/* callee-save GPRs */
	stw	14, (1+7)*4(3)	/* xxx: block move */
	stw	15, (2+7)*4(3)
	stw	16, (3+7)*4(3)
	stw	17, (4+7)*4(3)
	stw	18, (5+7)*4(3)
	stw	19, (6+7)*4(3)
	stw	20, (7+7)*4(3)
	stw	21, (8+7)*4(3)
	stw	22, (9+7)*4(3)
	stw	23, (10+7)*4(3)
	stw	24, (11+7)*4(3)
	stw	25, (12+7)*4(3)
	stw	26, (13+7)*4(3)
	stw	27, (14+7)*4(3)
	stw	28, (15+7)*4(3)
	stw	29, (16+7)*4(3)
	stw	30, (17+7)*4(3)
	stw	31, (18+7)*4(3)

	li	3, 0			/* return */
	blr


._setmcontext:
	lwz	13, (0+7)*4(3)	/* callee-save GPRs */
	lwz	14, (1+7)*4(3)	/* xxx: block move */
	lwz	15, (2+7)*4(3)
	lwz	16, (3+7)*4(3)
	lwz	17, (4+7)*4(3)
	lwz	18, (5+7)*4(3)
	lwz	19, (6+7)*4(3)
	lwz	20, (7+7)*4(3)
	lwz	21, (8+7)*4(3)
	lwz	22, (9+7)*4(3)
	lwz	23, (10+7)*4(3)
	lwz	24, (11+7)*4(3)
	lwz	25, (12+7)*4(3)
	lwz	26, (13+7)*4(3)
	lwz	27, (14+7)*4(3)
	lwz	28, (15+7)*4(3)
	lwz	29, (16+7)*4(3)
	lwz	30, (17+7)*4(3)
	lwz	31, (18+7)*4(3)

	lwz	1, 4*4(3)
	lwz	2, 5*4(3)

	lwz	0, 0*4(3)
	mtlr	0
	lwz	0, 1*4(3)
	mtcr	0			/* mtcrf 0xFF, r0 */
	lwz	0, 2*4(3)
	mtctr	0
	lwz	0, 3*4(3)
	mtxer	0

	lwz	3,	6*4(3)
	blr

