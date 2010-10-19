/* Minimal cooperative (non-preemptive) threading library */
#ifndef LIBCO_H
#define LIBCO_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

   /* Reference to a cooperative thread */
   typedef void* cothread_t;

   /* Sets size of main stack and stack space for other threads in portable version.
      Defaults to 64K and 256K, respectively. Must be set before first call to
      co_create(), otherwise it has no effect. */
   void co_setmax( uint64_t main_size, uint64_t total_stack );

   /* Currently active thread. Before first call to co_switch(), returns main thread. */
   cothread_t co_active( void );

   /* Creates new thread with stack of size bytes and executes entry() when next
      switched to. Returns pointer to thread, or NULL if it failed. User entry()
      function must NEVER return. */ 
   cothread_t co_create( uint64_t size, void (*entry)( void ) );

   /* Switches to thread. Thread must not already be the active one. */
   void co_switch( cothread_t thread );

   /* Deletes thread. Thread must not be the active one or main thread. */
   void co_delete( cothread_t thread );

#ifdef __cplusplus
}
#endif

#endif
