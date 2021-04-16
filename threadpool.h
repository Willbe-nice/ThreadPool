#ifndef THREADPOOL_H
#define THREADPOOL_H
#include "queue.h"
#include <pthread.h>

typedef void (*EnterFP)(void*);

typedef struct ThreadPool
{
	int thread_cnt;
	pthread_t* tids;
	Queue* store;
	EnterFP enter;
	pthread_mutex_t hlock;
	pthread_mutex_t tlock;
	pthread_cond_t empty;
	pthread_cond_t full;
}ThreadPool;

ThreadPool* create_threadpool(int thread_cnt,int store_cal,EnterFP enter);

void start_threadpool(ThreadPool* thread);

void push_threadpool(ThreadPool* thread,void* task);

void* pop_threadpool(ThreadPool* thread);

void destroy_threadpool(ThreadPool* thread);

#endif//THREADPOOL_H
