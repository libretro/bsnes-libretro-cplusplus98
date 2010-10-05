/* Extended cooperative threading interface with lots of conveniences */

#ifndef LIBCO_EX_H
#define LIBCO_EX_H

#include "libco.h"

#ifdef __cplusplus
	extern "C" {
#endif

/* Sets size of main stack and stack space for other threads in portable version.
Defaults to 64K and 256K, respectively. Must be set before first call to
co_create(), otherwise it has no effect. */
void co_abi co_set_stack( unsigned main_stack, unsigned total_stack );


typedef void (*co_entry_t)( intptr_t user_data );

/* Same as co_create(), except it calls user function with data */
cothread_t co_abi co_create2( unsigned stack, co_entry_t func, uintptr_t user_data );


/* Private */
extern unsigned co_main_stack_;
extern unsigned co_total_stack_;

#ifdef __cplusplus
	}
#endif

#endif
