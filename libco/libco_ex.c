/* Extended cooperative threading interface with lots of conveniences */

#include "libco_ex.h"

unsigned co_main_stack_  = 128 * 1024L;
unsigned co_total_stack_ = 128 * 1024L;

void co_abi co_set_stack( unsigned main_stack, unsigned total_stack )
{
   co_main_stack_  = main_stack;
   co_total_stack_ = total_stack;
}

static volatile co_entry_t user_entry;
static volatile intptr_t   user_data;
static volatile cothread_t prev_thread;

static void entry_( void )
{
   co_entry_t func = user_entry;
   intptr_t   data = user_data;
   co_switch( prev_thread );
   func( data );
}

cothread_t co_abi co_create2( unsigned stack, co_entry_t func, uintptr_t data )
{
   cothread_t t = co_create( stack, entry_ );
   if ( t )
   {
      user_entry = func;
      user_data  = data;
      prev_thread = co_active();
      co_switch( t );
   }
   return t;
}
