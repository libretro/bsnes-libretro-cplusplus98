#include "libco.h"

void co_switch(cothread_t t)
{
   cothread_t cur = g_current;
   g_current = t;

   pthread_cond_signal(t->cond);

   pthread_cond_wait(cur->cond);
   if (!cur->active)
      pthread_exit(NULL);
}

void co_delete(cothread_t t)
{
   t->active = 0;
   pthread_cond_signal(t->cond);
   pthread_join(t->thread, NULL);
}

cothread_t co_active(void)
{
   if (g_first)
      allocate_first();

   return g_current;
}

static void* Entry(void* in_data)
{
   cothread_data_t *data = in_data;
   pthread_cond_wait(data->cond);
   data->entry();
}

cothread_t co_create(unsigned int heapsize, void (*entry)(void))
{




}



