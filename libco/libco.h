/* Minimal cooperative (non-preemptive) threading library */

/* libco 0.12 (2008-01-07) */
#ifndef LIBCO_H
#define LIBCO_H

#include <stdint.h>

#ifdef __cplusplus
	extern "C" {
#endif

#if defined(__i386__) || defined(_MSC_VER)
  #if defined(__GNUC__)
    #if defined(__NetBSD__) || defined(__OpenBSD__)
      #define co_abi __attribute__((regparm(2)))
    #else
      #define co_abi __attribute__((fastcall))
    #endif
  #elif defined(_MSC_VER)
    #define co_abi __fastcall
  #endif
#else
  #define co_abi
#endif

/* Reference to a cooperative thread */
typedef struct cothread_t_* cothread_t;

/* Sets size of main stack and stack space for other threads in portable version.
Defaults to 64K and 256K, respectively. Must be set before first call to
co_create(), otherwise it has no effect. */
void co_abi co_setmax( unsigned main_size, unsigned total_stack );

/* Currently active thread. Before first call to co_switch(), returns main thread. */
cothread_t co_abi co_active( void );

/* Creates new thread with stack of size bytes and executes entry() when next
switched to. Returns pointer to thread, or NULL if it failed. User entry()
function must NEVER return. */ 
cothread_t co_abi co_create( unsigned int size, void (*entry)( void ) );

/* Switches to thread. Thread must not already be the active one. */
void co_abi co_switch( cothread_t thread );

/* Deletes thread. Thread must not be the active one or main thread. */
void co_abi co_delete( cothread_t thread );

#ifdef __cplusplus
	}
#endif

#endif
