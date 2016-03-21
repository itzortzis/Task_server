#include <stdio.h>
#include <stdlib.h>
#include "task_server.h"
#include "queue.h"



void print_queue(queue q)
{
	printf("\n");
	printf("Queue:\n");
	printf("-------\n");
	printf("\n");
	printf("  head: %d\n", q.head);
	printf("  tail: %d\n", q.tail);
	printf("  thread_id: %ld\n", q.thread_id);
	int i;
	for (i=0; i<q.tail; i++) {
		printf("  tasks[%d] = %d\n", i, q.tasks[i]);
	}
	printf("\n");
}


queue init_queue(queue q, long id)
{
	q.head = 0;
	q.tail = 0;
	q.shutdown = 0;
	q.thread_id = id;
	int i;
	q.tasks = (int *)malloc(QUEUE_SIZE*sizeof(int));
	for (i=0; i<QUEUE_SIZE; i++) {
		q.tasks[i] = UNKNOWN_TASK;
	}
	return q;
}


queue enqueue(queue q, int task)
{
	if (q.tail < QUEUE_SIZE) {
		q.tasks[q.tail] = task;
		q.tail++;
	}
	else {
		printf("Queue of thread %ld is full!\n", q.thread_id);
	}
	return q;
}


queue dequeue(queue q) {
	//int out_task;
	if (q.tail == q.head) {
		//printf("Queue of thread %ld is empty!\n", q.thread_id);
		//out_task = UNKNOWN_TASK;
	}
	else {
		//out_task = q.tasks[q.head];
		int i;
		for (i=q.head+1; i<q.tail; i++) {
			q.tasks[i-1] = q.tasks[i];
		}
		q.tasks[q.tail] = UNKNOWN_TASK;
		q.tail--;

	}
	return q;
}