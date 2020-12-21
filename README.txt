===============
* Task Server *
===============


Description:
-------------

This repository was created for the project task_server that contains a
multi-threading server program in C programming language. This server entrusts
heavy tasks to workers-processes(threads). The server has a dynamic funcionality, so it 
can create or kill threads if it is needed. Every worker has a queue, in which all the 
related tasks are stored. The server is watching all the queues and adjusts the load or 
create/kill threads.


Files:
-------

queue.c, queue.h : the queue - array based implementation
task_server.c    : the server program
task_server.h    : auxiliary library for the server program
workers.c        : worker tasks functions
workers.h        : auxiliary library for workers.c 


Settings:
----------

The following macros are located in task_server.h file and they are crucial for
the results of the program execution.

1. MEAN_TAIL_THRESHOLD : if a worker's queue contains more tasks than this 
   threshold, then the server creates a new thread.

2. NUM_TASKS : number of tasks to be generated.

3. IDLE_TIME_THRESHOLD : if some threads are in idle status for a long time 
   then the server has to kill a thread. 

4. WORK_LOAD_THRESHOLD : maximum number of tasks that a worker can store.


Execute:
---------

1. run make in terminal
2. run ./task_server in terminal
3. Ctrl-c to stop the program 
