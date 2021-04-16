#include <stdlib.h>
#include "queue.h"

// 创建队列
Queue* create_queue(int cal)
{
	Queue* queue = malloc(sizeof(Queue));
	queue->arr = malloc(sizeof(void*)*cal+1);
	queue->cal = cal;
	queue->front = 0;
	queue->rear = 0;
	return queue;
}

// 销毁队列
void destroy_queue(Queue* queue)
{
	free(queue->arr);
	free(queue);
}

// 入队
void push_queue(Queue* queue,void* arg)
{
	queue->arr[queue->rear++] = arg;
	queue->rear %= queue->cal;
}

// 出队
void pop_queue(Queue* queue)
{
	queue->front = (queue->front+1)%queue->cal;
}

// 队空
bool empty_queue(Queue* queue)
{
	return queue->front == queue->rear;
}

// 队满
bool full_queue(Queue* queue)
{
	return (queue->rear+1)%queue->cal == queue->front;
}

// 队头
void* front_queue(Queue* queue)
{
	return queue->arr[queue->front];
}

// 队尾
void* rear_queue(Queue* queue)
{
	return queue->arr[(queue->rear+queue->cal-1)%queue->cal];
}
