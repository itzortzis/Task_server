#ifndef _task_server_h
#define _task_server_h

#define TRUE 1
#define FALSE 0

#define TYPE_1 1
#define TYPE_2 2
#define TYPE_3 3
#define Y_SMALL 89
#define Y_BIG 121 
#define TYPES 3
#define OFFSET 1
#define ON 0
#define OFF 1

#define WORK_LOAD_THRESHOLD 8

#define NUM_TASKS 3000

#define IDLE_TIME_THRESHOLD 0.0003
#define MEAN_TAIL_THRESHOLD 5

#define Tp 1000
#define Tc 1000


void *worker_basic_function(void *);

void init_thread_ids();

void init_thread_queues();

void *task_generator_handler();

void quit_signal_handler(int);

void create_new_thread();

void *aux_function();

#endif
