#ifndef QUEUE_H
#define QUEUE_H
#include <stdbool.h>

typedef struct Queue
{
	void** arr;
	int cal;
	int front;
	int rear;
}Queue;

// 创建队列
Queue* create_queue(int cal);

// 销毁队列
void destroy_queue(Queue* queue);

// 入队
void push_queue(Queue* queue,void* arg);

// 出队
void pop_queue(Queue* queue);

// 队空
bool empty_queue(Queue* queue);

// 队满
bool full_queue(Queue* queue);

// 队头
void* front_queue(Queue* queue);

// 队尾
void* rear_queue(Queue* queue);
#endif//QUEUE_H
