#include "libco.h"
#include <setjmp.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct
{
   jmp_buf jmpbuf;
   void (*entry)(void);
   int called;
} cothread_data;

static cothread_data g_main = {
   .called = 1
};
static cothread_data *g_current = &g_main;
static volatile void* pitchfork; // hurr, durr

static void spring(void (*entry)(void))
{
   volatile char foo[0x10000];
   pitchfork = foo + 0x10000 - 1; // avoids optimization
   entry();
}

void co_switch(cothread_t co)
{
   fprintf(stderr, "co_switch!\n");
   cothread_data *current = g_current;
   g_current = co;
   if (!setjmp(current->jmpbuf))
   {
      if (!g_current->called)
      {
         fprintf(stderr, "calling spring()!\n");
         g_current->called = 1;
         spring(g_current->entry);
      }
      else
         longjmp(g_current->jmpbuf, 1);
   }
}

void co_delete(cothread_t co)
{
   free(co);
}

cothread_t co_create(unsigned int size, void (*entry)(void))
{
   cothread_data *data = calloc(1, sizeof(*data));
   data->entry = entry;
   return data;
}

cothread_t co_active(void)
{
   return g_current;
}



