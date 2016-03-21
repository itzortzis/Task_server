===============
* Task Server *
===============


Description:
-------------

This repository was created for the project task_server, which implements a
multi-threading server program in C programming language. This server entrusts
heavy tasks to workers (threads). The server has a dynamic funcionality, so it 
can create or kill threads if it is needed. Every worker has a queue that 
contains the tasks that it has to complete. The server is watching all the 
queues of the workers and adjusts the load or create/kill a thread and so on.


Files:
-------

queue.c, queue.h : library that implements a queue and its basic functions.
task_server.c    : the server program
task_server.h    : auxiliary library for the server program
workers.c        : contains the functions for two types of tasks
workers.h        : auxiliary library for workers.c 


Settings:
----------

The following macros are in task_server.h file and they are too important for
the excecution of the program.

1. MEAN_TAIL_THRESHOLD : if the size of a worker's queue is bigger than this 
   threshold then the server creates a new thread.

2. NUM_TASKS : number of tasks to be generated.

3. IDLE_TIME_THRESHOLD : if some threads are in idle status for a long time 
   then the server has to kill a thread. 

4. WORK_LOAD_THRESHOLD : maximum number of tasks that a worker can store.


Execute:
---------

1. run make in terminal
2. run ./task_server in terminal
3. Ctrl-c to stop the program 
