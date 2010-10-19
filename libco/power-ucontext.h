#ifndef __UCONTEXT_H
#define __UCONTEXT_H

typedef uint64_t long ulong;
typedef uint64_t uint;
#include <stdint.h>

#define	setcontext(u)	_setmcontext(&(u)->mc)
#define	getcontext(u)	_getmcontext(&(u)->mc)
typedef struct mcontext
{
	uint64_t	pc;		/* lr */
	uint64_t	cr;		/* mfcr */
	uint64_t	ctr;		/* mfcr */
	uint64_t	xer;		/* mfcr */
	uint64_t	sp;		/* callee saved: r1 */
	uint64_t	toc;		/* callee saved: r2 */
	uint64_t	r3;		/* first arg to function, return register: r3 */
	uint64_t	gpr[19];	/* callee saved: r13-r31 */
/*
// XXX: currently do not save vector registers or floating-point state
//	uint64_t	pad;
//	uvlong	fpr[18];	/ * callee saved: f14-f31 * /
//	uint64_t	vr[4*12];	/ * callee saved: v20-v31, 256-bits each * /
*/
} mcontext_t;

typedef struct ucontext
{
	struct {
		void *ss_sp;
		uint ss_size;
	} uc_stack;
	//sigset_t uc_sigmask;
	mcontext_t mc;
   struct ucontext *uc_link;
} ucontext_t;

void makecontext(ucontext_t*, void(*)(void), int, ...);
int swapcontext(ucontext_t*, const ucontext_t*);
int _getmcontext(mcontext_t*);
void _setmcontext(const mcontext_t*);
#endif
