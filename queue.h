#ifndef _queue_h
#define _queue_h

#define QUEUE_SIZE 10
#define UNKNOWN_TASK 200
#define EMPTY_QUEUE 0

typedef struct {
	int head, tail;
	long thread_id;
	int shutdown;
	int *tasks;
}queue;

void print_queue(queue);

queue init_queue(queue, long);

queue enqueue(queue, int);

queue dequeue(queue);





#endif