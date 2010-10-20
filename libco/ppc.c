#include "libco.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "debug.h"

/* Code in external asm file */
#if LIBCO_PPC_ASM

#ifdef __cplusplus
	extern "C"
#endif

void co_swap_asm( cothread_t, cothread_t );
#define CO_SWAP_ASM( x, y ) co_swap_asm( x, y )

/* Code right here */
#else

static const uint32_t code [] = {
#if LIBCO_PPC32
	0x7d000026, /* mfcr    r8 */
	0x80a30000, /* lwz     r5,0(r3) */
	0x90240010, /* stw     r1,16(r4) */
	0x7d2802a6, /* mflr    r9 */
	0x91c40028, /* stw     r14,40(r4) */
	0x91e4002c, /* stw     r15,44(r4) */
	0x92040030, /* stw     r16,48(r4) */
	0x92240034, /* stw     r17,52(r4) */
	0x92440038, /* stw     r18,56(r4) */
	0x9264003c, /* stw     r19,60(r4) */
	0x92840040, /* stw     r20,64(r4) */
	0x92a40044, /* stw     r21,68(r4) */
	0x92c40048, /* stw     r22,72(r4) */
	0x92e4004c, /* stw     r23,76(r4) */
	0x93040050, /* stw     r24,80(r4) */
	0x93240054, /* stw     r25,84(r4) */
	0x93440058, /* stw     r26,88(r4) */
	0x9364005c, /* stw     r27,92(r4) */
	0x93840060, /* stw     r28,96(r4) */
	0x93a40064, /* stw     r29,100(r4) */
	0x93c40068, /* stw     r30,104(r4) */
	0x93e4006c, /* stw     r31,108(r4) */
	0x91240008, /* stw     r9,8(r4) */
	0x80e30008, /* lwz     r7,8(r3) */
	0x80230010, /* lwz     r1,16(r3) */
	0x91040004, /* stw     r8,4(r4) */
	0x80c30004, /* lwz     r6,4(r3) */
	0x7ce803a6, /* mtlr    r7 */
	0x81c30028, /* lwz     r14,40(r3) */
	0x81e3002c, /* lwz     r15,44(r3) */
	0x82030030, /* lwz     r16,48(r3) */
	0x82230034, /* lwz     r17,52(r3) */
	0x82430038, /* lwz     r18,56(r3) */
	0x8263003c, /* lwz     r19,60(r3) */
	0x82830040, /* lwz     r20,64(r3) */
	0x82a30044, /* lwz     r21,68(r3) */
	0x82c30048, /* lwz     r22,72(r3) */
	0x82e3004c, /* lwz     r23,76(r3) */
	0x83030050, /* lwz     r24,80(r3) */
	0x83230054, /* lwz     r25,84(r3) */
	0x83430058, /* lwz     r26,88(r3) */
	0x8363005c, /* lwz     r27,92(r3) */
	0x83830060, /* lwz     r28,96(r3) */
	0x83a30064, /* lwz     r29,100(r3) */
	0x83c30068, /* lwz     r30,104(r3) */
	0x83e3006c, /* lwz     r31,108(r3) */
	0x7ccff120, /* mtcr    r6 */
#else
	0x7d000026, /* mfcr    r8 */
	0x80a30000, /* lwz     r5,0(r3) */
	0xf8240010, /* std     r1,16(r4) */
	0x7d2802a6, /* mflr    r9 */
	0xf9c40030, /* std     r14,48(r4) */
	0xf9e40038, /* std     r15,56(r4) */
	0xfa040040, /* std     r16,64(r4) */
	0xfa240048, /* std     r17,72(r4) */
	0xfa440050, /* std     r18,80(r4) */
	0xfa640058, /* std     r19,88(r4) */
	0xfa840060, /* std     r20,96(r4) */
	0xfaa40068, /* std     r21,104(r4) */
	0xfac40070, /* std     r22,112(r4) */
	0xfae40078, /* std     r23,120(r4) */
	0xfb040080, /* std     r24,128(r4) */
	0xfb240088, /* std     r25,136(r4) */
	0xfb440090, /* std     r26,144(r4) */
	0xfb640098, /* std     r27,152(r4) */
	0xfb8400a0, /* std     r28,160(r4) */
	0xfba400a8, /* std     r29,168(r4) */
	0xfbc400b0, /* std     r30,176(r4) */
	0xfbe400b8, /* std     r31,184(r4) */
	0xf9240008, /* std     r9,8(r4) */
	0xe8e30008, /* ld      r7,8(r3) */
	0xe8230010, /* ld      r1,16(r3) */
	0x91040004, /* stw     r8,4(r4) */
	0x80c30004, /* lwz     r6,4(r3) */
	0x7ce803a6, /* mtlr    r7 */
	0xe9c30030, /* ld      r14,48(r3) */
	0xe9e30038, /* ld      r15,56(r3) */
	0xea030040, /* ld      r16,64(r3) */
	0xea230048, /* ld      r17,72(r3) */
	0xea430050, /* ld      r18,80(r3) */
	0xea630058, /* ld      r19,88(r3) */
	0xea830060, /* ld      r20,96(r3) */
	0xeaa30068, /* ld      r21,104(r3) */
	0xeac30070, /* ld      r22,112(r3) */
	0xeae30078, /* ld      r23,120(r3) */
	0xeb030080, /* ld      r24,128(r3) */
	0xeb230088, /* ld      r25,136(r3) */
	0xeb430090, /* ld      r26,144(r3) */
	0xeb630098, /* ld      r27,152(r3) */
	0xeb8300a0, /* ld      r28,160(r3) */
	0xeba300a8, /* ld      r29,168(r3) */
	0xebc300b0, /* ld      r30,176(r3) */
	0xebe300b8, /* ld      r31,184(r3) */
	0x7ccff120, /* mtcr    r6 */
#endif

#if LIBCO_PPC_FP
	0xd9c400e0, /* stfd    f14,224(r4) */
	0xd9e400e8, /* stfd    f15,232(r4) */
	0xda0400f0, /* stfd    f16,240(r4) */
	0xda2400f8, /* stfd    f17,248(r4) */
	0xda440100, /* stfd    f18,256(r4) */
	0xda640108, /* stfd    f19,264(r4) */
	0xda840110, /* stfd    f20,272(r4) */
	0xdaa40118, /* stfd    f21,280(r4) */
	0xdac40120, /* stfd    f22,288(r4) */
	0xdae40128, /* stfd    f23,296(r4) */
	0xdb040130, /* stfd    f24,304(r4) */
	0xdb240138, /* stfd    f25,312(r4) */
	0xdb440140, /* stfd    f26,320(r4) */
	0xdb640148, /* stfd    f27,328(r4) */
	0xdb840150, /* stfd    f28,336(r4) */
	0xdba40158, /* stfd    f29,344(r4) */
	0xdbc40160, /* stfd    f30,352(r4) */
	0xdbe40168, /* stfd    f31,360(r4) */
	0xc9c300e0, /* lfd     f14,224(r3) */
	0xc9e300e8, /* lfd     f15,232(r3) */
	0xca0300f0, /* lfd     f16,240(r3) */
	0xca2300f8, /* lfd     f17,248(r3) */
	0xca430100, /* lfd     f18,256(r3) */
	0xca630108, /* lfd     f19,264(r3) */
	0xca830110, /* lfd     f20,272(r3) */
	0xcaa30118, /* lfd     f21,280(r3) */
	0xcac30120, /* lfd     f22,288(r3) */
	0xcae30128, /* lfd     f23,296(r3) */
	0xcb030130, /* lfd     f24,304(r3) */
	0xcb230138, /* lfd     f25,312(r3) */
	0xcb430140, /* lfd     f26,320(r3) */
	0xcb630148, /* lfd     f27,328(r3) */
	0xcb830150, /* lfd     f28,336(r3) */
	0xcba30158, /* lfd     f29,344(r3) */
	0xcbc30160, /* lfd     f30,352(r3) */
	0xcbe30168, /* lfd     f31,360(r3) */
#endif

#if LIBCO_PPC_ALTIVEC
	0x7ca042a6, /* mfvrsave r5 */
	0x39040180, /* addi    r8,r4,384 */
	0x39240190, /* addi    r9,r4,400 */
	0x70a00fff, /* andi.   r0,r5,4095 */
	0x90a40018, /* stw     r5,24(r4) */
	0x41a2005c, /* beq+    10000abc <co_swap_asm+0x1bc> */
	0x7e8041ce, /* stvx    v20,r0,r8 */
	0x39080020, /* addi    r8,r8,32 */
	0x7ea049ce, /* stvx    v21,r0,r9 */
	0x39290020, /* addi    r9,r9,32 */
	0x7ec041ce, /* stvx    v22,r0,r8 */
	0x39080020, /* addi    r8,r8,32 */
	0x7ee049ce, /* stvx    v23,r0,r9 */
	0x39290020, /* addi    r9,r9,32 */
	0x7f0041ce, /* stvx    v24,r0,r8 */
	0x39080020, /* addi    r8,r8,32 */
	0x7f2049ce, /* stvx    v25,r0,r9 */
	0x39290020, /* addi    r9,r9,32 */
	0x7f4041ce, /* stvx    v26,r0,r8 */
	0x39080020, /* addi    r8,r8,32 */
	0x7f6049ce, /* stvx    v27,r0,r9 */
	0x39290020, /* addi    r9,r9,32 */
	0x7f8041ce, /* stvx    v28,r0,r8 */
	0x39080020, /* addi    r8,r8,32 */
	0x7fa049ce, /* stvx    v29,r0,r9 */
	0x39290020, /* addi    r9,r9,32 */
	0x7fc041ce, /* stvx    v30,r0,r8 */
	0x7fe049ce, /* stvx    v31,r0,r9 */
	0x80a30018, /* lwz     r5,24(r3) */
	0x39030180, /* addi    r8,r3,384 */
	0x39230190, /* addi    r9,r3,400 */
	0x70a00fff, /* andi.   r0,r5,4095 */
	0x7ca043a6, /* mtvrsave r5 */
	0x4da20020, /* beqlr+   */
	0x7e8040ce, /* lvx     v20,r0,r8 */
	0x39080020, /* addi    r8,r8,32 */
	0x7ea048ce, /* lvx     v21,r0,r9 */
	0x39290020, /* addi    r9,r9,32 */
	0x7ec040ce, /* lvx     v22,r0,r8 */
	0x39080020, /* addi    r8,r8,32 */
	0x7ee048ce, /* lvx     v23,r0,r9 */
	0x39290020, /* addi    r9,r9,32 */
	0x7f0040ce, /* lvx     v24,r0,r8 */
	0x39080020, /* addi    r8,r8,32 */
	0x7f2048ce, /* lvx     v25,r0,r9 */
	0x39290020, /* addi    r9,r9,32 */
	0x7f4040ce, /* lvx     v26,r0,r8 */
	0x39080020, /* addi    r8,r8,32 */
	0x7f6048ce, /* lvx     v27,r0,r9 */
	0x39290020, /* addi    r9,r9,32 */
	0x7f8040ce, /* lvx     v28,r0,r8 */
	0x39080020, /* addi    r8,r8,32 */
	0x7fa048ce, /* lvx     v29,r0,r9 */
	0x39290020, /* addi    r9,r9,32 */
	0x7fc040ce, /* lvx     v30,r0,r8 */
	0x7fe048ce, /* lvx     v31,r0,r9 */
#endif

	0x4e800020, /* blr */
};

/* PPC32 function pointers go directly to code */
#if LIBCO_PPC32
#define CO_SWAP_ASM( x, y ) \
	((void (*)( cothread_t, cothread_t )) (uintptr_t) code)( x, y )

/* PPC64 function pointers go to descriptor */
#else
static const void* code_descriptor [3];
#define CO_SWAP_ASM( x, y ) \
	((void (*)( cothread_t, cothread_t )) (uintptr_t) code_descriptor)( x, y )

#endif

#endif

int const state_size  = 1024;
int const above_stack = 1024;
int const stack_align = 256;

#ifndef thread_local
	#define thread_local
#endif

static thread_local cothread_t co_active_handle = 0;
static int co_flags;

static uint32_t* co_create_( unsigned size )
{
   SNES_DBG("co_create_()\n");
	uint32_t* t = (uint32_t*) memalign(128, size);
	
	if ( t )
	{
		memset( t, 0, state_size );
		*t = co_flags;
	}
	
   SNES_DBG("returning from co_create_()\n");
	return t;
}

cothread_t co_create( unsigned int size, void (*entry_)( void ) )
{
   SNES_DBG("co_create()\n");
	uint32_t* t = NULL;
	
	size += state_size + above_stack + stack_align;
	
	/* Be sure main thread exists */
	if ( co_active() )
		t = co_create_( size );
	
	if ( t )
	{
		uintptr_t sp;
		uintptr_t entry = (uintptr_t) entry_;
		int shift;
		
		/* Save current registers into new thread */
		CO_SWAP_ASM( t, t );
		
		/* Different ABIs make pointers different sizes, so we manually store
		pointers as always 64 bits. */
		
		/* On 32-bit machine, >>32 is undefined behavior, so we do two shifts. */
		#if LIBCO_PPC32
			shift = 0;
		#else
			shift = 16;
		#endif
		
		#if !LIBCO_PPC32
			entry = *(uintptr_t*) entry;
		#endif
		
		t [2] = (uint32_t) (entry >> shift >> shift);
		t [3] = (uint32_t) entry;
		
		sp = (uintptr_t) t + size - above_stack;
		sp -= sp % stack_align;
		t [4] = (uint32_t) (sp >> shift >> shift); 
		t [5] = (uint32_t) sp;
	}
	
   SNES_DBG("returning from co_create()\n");
	return t;
}

void co_delete( cothread_t t )
{
   SNES_DBG("co_delete()\n");
	free( t );
   SNES_DBG("returning from co_delete()\n");
}

//static int co_switch_cnt = 0;

cothread_t co_active()
{
   SNES_DBG("co_active()\n");
	if ( !co_active_handle )
	{
		co_active_handle = co_create_( state_size );
		#if !LIBCO_PPC_ASM && !LIBCO_PPC32
		{
			/* Our descriptor needs the same TOC pointer, so just copy another and
			modify the function pointer */
			memcpy( code_descriptor, (void*) (uintptr_t) &co_delete, sizeof code_descriptor );
			code_descriptor [0] = code;
		}
		#endif
	}
	
   SNES_DBG("returning from co_active(): %p. co_switch count: %d\n", co_active_handle, co_switch_cnt);
	return co_active_handle;
}

void co_switch( cothread_t t )
{
   //co_switch_cnt++;
   //SNES_DBG("co_switch()\n");
	cothread_t old = co_active_handle;
	co_active_handle = t;
	
	CO_SWAP_ASM( t, old );
   //SNES_DBG("returning from co_switch()\n"); // This happens in a different cothread.
}
