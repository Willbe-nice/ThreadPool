#include <stdio.h>
#include <stdlib.h>
#include "threadpool.h"

// 线程入口函数
static void* run(void* arg)
{
	ThreadPool* thread = arg;
	for(;;)
	{
		void* task = pop_threadpool(thread);
		thread->enter(task);
	}
}

ThreadPool* create_threadpool(int thread_cnt,int store_cal,EnterFP enter)
{
	ThreadPool* thread = malloc(sizeof(ThreadPool));
	thread->tids = malloc(sizeof(pthread_t)*thread_cnt);
	thread->store = create_queue(store_cal);
	thread->thread_cnt = thread_cnt;
	thread->enter = enter;

	pthread_mutex_init(&thread->hlock,NULL);
	pthread_mutex_init(&thread->tlock,NULL);
	pthread_cond_init(&thread->empty,NULL);
	pthread_cond_init(&thread->full,NULL);
	
	return thread;
}

void start_threadpool(ThreadPool* thread)
{
	for(int i=0; i<thread->thread_cnt; i++)
	{
		pthread_create(thread->tids+i,NULL,run,thread);
	}
}

void push_threadpool(ThreadPool* thread,void* task)
{
	pthread_mutex_lock(&thread->tlock);
	while(full_queue(thread->store))
	{
		pthread_cond_signal(&thread->empty);
		pthread_cond_wait(&thread->full,&thread->tlock);
	}
	printf("add task %p\n",task);
	
	push_queue(thread->store,task);
	pthread_cond_signal(&thread->empty);
	pthread_mutex_unlock(&thread->tlock);
}

void* pop_threadpool(ThreadPool* thread)
{
	pthread_mutex_lock(&thread->hlock);
	while(empty_queue(thread->store))
	{
		pthread_cond_signal(&thread->full);
		pthread_cond_wait(&thread->empty,&thread->hlock);
	}
	
	void* task = front_queue(thread->store);
	pop_queue(thread->store);
	printf("pop task %p\n",task);
	pthread_cond_signal(&thread->full);
	pthread_mutex_unlock(&thread->hlock);
	return task;
}

void destroy_threadpool(ThreadPool* thread)
{
	pthread_cond_destroy(&thread->full);	
	pthread_cond_destroy(&thread->empty);
	pthread_mutex_destroy(&thread->tlock);
	pthread_mutex_destroy(&thread->hlock);
	destroy_queue(thread->store);
	free(thread->tids);
	free(thread);
}
