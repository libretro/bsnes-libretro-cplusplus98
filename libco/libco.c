#include "libco.h"
#include <pthread.h>
#include <string.h>
#include <malloc.h>
#include <stdio.h>

typedef struct 
{
   pthread_t thread;
   pthread_cond_t cond;
   pthread_mutex_t cond_lock;
   volatile int active;
   volatile int can_sleep;
   void (*entry)(void);
} cothread_data_t;

static cothread_data_t *g_main_data;
static cothread_data_t *g_current;
static int g_first = 1;

static void allocate_first(void)
{
   fprintf(stderr, "main thread is %zu\n", (size_t)pthread_self());
   g_current = calloc(1, sizeof(cothread_data_t));
   pthread_mutex_init(&g_current->cond_lock, NULL);
   pthread_cond_init(&g_current->cond, NULL);
   g_current->active = 1;
   g_current->can_sleep = 1;
   g_current->entry = NULL;
   g_current->thread = pthread_self();
   g_first = 0;
   g_main_data = g_current;
}


void co_switch(cothread_t t)
{
   fprintf(stderr, "co_switch() in thread %zu\n", (size_t)pthread_self());

   cothread_data_t *cur = g_current;
   g_current = t;

   pthread_cond_signal(&g_current->cond);
   pthread_mutex_lock(&cur->cond_lock);

   //pthread_mutex_lock(&g_current->cond_lock);
   //g_current->can_sleep = 0;
   //pthread_mutex_unlock(&g_current->cond_lock);

   //if (cur->can_sleep)
   //{
      pthread_cond_wait(&cur->cond, &cur->cond_lock);
      pthread_mutex_unlock(&cur->cond_lock);
   //}
   //pthread_mutex_unlock(&cur->cond_lock);
   //cur->can_sleep = 1;
   if (!cur->active)
      pthread_exit(NULL);
}

void co_delete(cothread_t t)
{
   cothread_data_t *data = t;
   data->active = 0;
   pthread_cond_signal(&data->cond);
   pthread_join(data->thread, NULL);
   pthread_mutex_destroy(&data->cond_lock);
   pthread_cond_destroy(&data->cond);
   free(data);
}

cothread_t co_active(void)
{
   if (g_first)
      allocate_first();

   return g_current;
}

static void* co_entry(void* in_data)
{
   cothread_data_t *data = in_data;

   pthread_mutex_lock(&data->cond_lock);
   pthread_cond_wait(&data->cond, &data->cond_lock);
   pthread_mutex_unlock(&data->cond_lock);

   data->entry();
   pthread_exit(NULL);
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
   data->can_sleep = 1;
   pthread_create(&data->thread, NULL, co_entry, data);
   return data;
}



