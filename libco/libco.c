#include "libco.h"
#include <pthread.h>
#include <string.h>
#include <malloc.h>

typedef struct 
{
   pthread_t thread;
   pthread_cond_t cond;
   pthread_mutex_t cond_lock;
   int active;
   void (*entry)(void);
} cothread_data_t;

static cothread_data_t g_current;
static int g_first = 1;

void co_switch(cothread_t t)
{
   if (g_first)
      allocate_first();

   cothread_data_t cur = g_current;
   memcpy(&g_current, t, sizeof g_current);

   pthread_cond_signal(&g_current.cond);

   pthread_mutex_lock(&cur.cond_lock);
   pthread_cond_wait(&cur.cond, &cur.cond_lock);
   pthread_mutex_unlock(&cur.cond_lock);
   if (!cur->active)
      pthread_exit(NULL);
}

void co_delete(cothread_t t)
{
   if (g_first)
      allocate_first();

   cothread_data_t *data = t;
   data->active = 0;
   pthread_cond_signal(data->cond);
   pthread_join(data->thread, NULL);
   pthread_mutex_destroy(&data->cond_lock);
   pthread_cond_destroy(&data->cond);
   free(data);
}

cothread_t co_active(void)
{
   if (g_first)
      allocate_first();

   return &g_current;
}

static void* co_entry(void* in_data)
{
   cothread_data_t *data = in_data;
   pthread_cond_wait(data->cond);
   data->entry();
}

static void allocate_first(void)
{
   pthread_mutex_init(&g_current.cond_lock);
   pthread_cond_init(&g_current.cond);
   g_current.active = 1;
   g_current.entry = NULL;
   g_first = 0;
}

cothread_t co_create(unsigned int heapsize, void (*entry)(void))
{
   if (g_first)
      allocate_first();

   (void)heapsize;

   cothread_data_t *data = calloc(1, sizeof(*data));
   pthread_cond_init(&data->cond, NULL);
   pthread_mutex_init(&data->cond_lock, NULL);
   data->entry = entry;
   data->active = 1;
   pthread_t id;
   pthread_create(&id, NULL, co_entry, data);
   data->thread = id;
   return data;
}



