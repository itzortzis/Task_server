#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "task_server.h"
#include "queue.h"
#include "workers.h"
#include <time.h>
#include <signal.h>
#include <math.h>
#include <unistd.h>


int type = TYPE_1;        // type of the tas
//int flag = 0;             // auxiliary flag
int print_flag = ON;
int generated_tasks = 0;  // number of gen. tasks
int excecuted_tasks = 0;  // number of exec. tasks
int total_threads = 1;    // thread pointer
int thread_counter = 1;   // number of threads
int task_count = 0;

pthread_mutex_t worker_mutex;       // mutex for the workers
pthread_cond_t worker_cv, main_cv;  // cv for the workers
pthread_attr_t attr;                // attribute
pthread_t *threads;                 // threads array
long *thread_ids;                   // id's array
float *wait_sec, program_sec;	    // times
queue *thread_queue;                // array with queues
clock_t start_program, end_program; // sum time of the program



int main()
{
	int i, rc, j;
	signal(SIGINT, quit_signal_handler); // control-c signal


	start_program = clock();
	pthread_t aux_thread;
	pthread_t task_gen;

	threads = (pthread_t *)malloc(total_threads*sizeof(pthread_t));
	thread_ids = (long *)malloc(total_threads*sizeof(long));
	thread_queue = (queue *)malloc(total_threads*sizeof(queue));
	wait_sec = (float *)malloc(total_threads*sizeof(float));

	init_thread_ids();
	init_thread_queues();


	// Initializations
	pthread_mutex_init(&worker_mutex, NULL);
	pthread_cond_init(&worker_cv, NULL);
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

	// Creating the first worker
	pthread_create(&threads[total_threads-1], &attr,
		worker_basic_function, (void *)thread_ids[total_threads-1]);

	// Creating the auxiliary thread
	pthread_create(&aux_thread, &attr,
		aux_function, NULL);

	pthread_create(&task_gen, &attr, task_generator_handler, NULL);


	//while(j < NUM_TASKS){
	//	task_generator_handler();
	//	j++;
	//}

	//for (i=0; i<total_threads; i++) {
	//	if (thread_queue[i].shutdown == FALSE) {
	//		pthread_join(threads[i], NULL);
	//	}
	//}
	//pthread_join(aux_thread, NULL);
	pthread_join(task_gen, NULL);
	end_program = clock();
	program_sec = (float)(end_program - start_program) / CLOCKS_PER_SEC;
	printf("Generated tasks: %d\t Excecuted tasks: %d\n", generated_tasks, excecuted_tasks);
	printf("Active threads: %d\n", thread_counter);
	printf("Program excecution time: %lf\n", program_sec);
	printf("\n\nSettings: \n");
	printf("-----------\n\n");
	printf("\tTc: %d sec\n", Tc);
	printf("\tTp: %d sec\n", Tp);
	printf("\tNUM_TASKS: %d\n", NUM_TASKS);
	printf("\tMEAN_TAIL_THRESHOLD: %d\n", MEAN_TAIL_THRESHOLD);
	printf("\nTask Server finished\n\n");
	return 0;
}


void *worker_basic_function(void *t)
{
	long id = (long)t;
	clock_t start, end;
	int i, N, j;

	while(TRUE) {
		//j++;
		//task_generator_handler();
		pthread_mutex_lock(&worker_mutex);

		if((thread_queue[id-1].shutdown == TRUE) ||
				((task_count == NUM_TASKS) && (thread_queue[id-1].tail == EMPTY_QUEUE))) {
			pthread_mutex_unlock(&worker_mutex);
			thread_counter--;
			printf("Thread %ld exiting...\n", id);
			pthread_exit(NULL);
			break;
		}
		else {

			// Dequeue a task to be excecuted
			type = thread_queue[id-1].tasks[thread_queue[id-1].head];
			thread_queue[id-1] = dequeue(thread_queue[id-1]);
			//excecuted_tasks++;
			start = clock();
			while (thread_queue[id].tail == EMPTY_QUEUE) {
				//for (i=0; i<1000; i++) {
					//printf("...");
				//}
				// Waiting
				pthread_cond_wait(&worker_cv, &worker_mutex);
			}
			end = clock();
			//waiting time
			wait_sec[id-1] = wait_sec[id-1] + (float)(abs(end - start)) / CLOCKS_PER_SEC;

			switch (type) {
				int y,z;
				case (TYPE_1):
					N = rand() % 200; //Excecuting task of type 1
					prime_numbers(N);
					//excecuted_tasks++;
					break;
				case (TYPE_2):
					N = rand() % 5;  //Excecuting task of type 2
					for (i=0; i<1000; i++) {
						memory_copy(y, z, N);
					}
					break;
				case (TYPE_3):
					N = rand() % 200;  //Excecuting task of type 3
					prime_numbers(N);
					//excecuted_tasks++;
					break;
			}
			excecuted_tasks++;
			pthread_mutex_unlock(&worker_mutex);

		}
		//excecuted_tasks++;
	}
}


void *aux_function()
{
	int i;
	float sum, mean_wait, idle_time;
	float mean_tail;
	while(1) {
		pthread_mutex_lock(&worker_mutex);
		sum = 0;
		for (i=0; i<total_threads; i++) {
			if (thread_queue[i].shutdown == FALSE) {
				sum = sum + wait_sec[i];
			}
		}
		mean_wait = sum/thread_counter;
		idle_time = (mean_wait/(abs((clock()-start_program))))*100;
		if (print_flag == ON) {
			printf("Idle time: %lf /100\n", idle_time);
			printf("Mean waiting time: %lf\n", mean_wait);
		}

		sum = 0;
		for (i=0; i<total_threads; i++) {
			if (thread_queue[i].shutdown == FALSE) {
				sum = sum + thread_queue[i].tail;
			}
		}
		mean_tail = sum/thread_counter;

		if (print_flag == ON) {
			printf("Mean tail: %lf\n", mean_tail);
			printf("Current task: %d\n", generated_tasks);
			printf("Active threads: %d\n\n", thread_counter);
		}

		if (mean_tail > MEAN_TAIL_THRESHOLD) {
			create_new_thread();
		}

		if ((idle_time > IDLE_TIME_THRESHOLD) && (thread_counter > 0)) {
			for (i=0; i<total_threads; i++) {
				if (thread_queue[i].tail == EMPTY_QUEUE) {
					thread_queue[i].shutdown = TRUE;
					thread_counter--;
					break;
				}
			}
		}
		pthread_mutex_unlock(&worker_mutex);
		usleep(Tp);

	}
}


void init_thread_ids()
{
	int i;
	for (i=0; i<total_threads; i++) {
		thread_ids[i] = (long)(i+1);
	}
	//main_id = 20;
}


void init_thread_queues()
{
	int i;
	for (i=0; i<total_threads; i++) {
		thread_queue[i] = init_queue(thread_queue[i], (long)(i+1));

		//printf("dsadsadsa\n");
	}
}


void *task_generator_handler()
{
	//long id = (long)t;

	long random_thread;
	int random_task, i, flag;

	while (task_count <= NUM_TASKS) {
		usleep(Tc);
		pthread_mutex_lock(&worker_mutex);

		// Generating new task
		random_task = (rand() % TYPES) + OFFSET;
		random_thread = (long)((rand() % total_threads));

		generated_tasks++;

		if (thread_queue[random_thread].tail < WORK_LOAD_THRESHOLD) {
			//printf("FUCK YEAAAAH\n");
			thread_queue[random_thread] =
					enqueue(thread_queue[random_thread], random_task);
			//printf("\nMain thread\n");
			pthread_cond_signal(&worker_cv);

		}
		else {
			flag = 0;

			for (i=0; i<total_threads; i++) {
				if (thread_queue[i].tail < WORK_LOAD_THRESHOLD) {
					thread_queue[i] = enqueue(thread_queue[i], random_task);
					//printf("\nMain thread\n");
					flag = 1;
					//pthread_cond_signal(&worker_cv);
				}
			}
		}
		pthread_mutex_unlock(&worker_mutex);
		task_count++;
	}
}


void create_new_thread()
{
	total_threads++;
	thread_counter++;
	threads = realloc(threads, total_threads*sizeof(pthread_t));

	thread_ids = realloc(thread_ids, total_threads*sizeof(long));
	//printf("Realloc2\n");
	thread_ids[total_threads-1] = total_threads;

	thread_queue = realloc(thread_queue, total_threads*sizeof(queue));
	thread_queue[total_threads-1] =
			init_queue(thread_queue[total_threads-1], total_threads);

	wait_sec = realloc(wait_sec, total_threads*sizeof(float));

	pthread_create(&threads[total_threads-1], &attr,
			worker_basic_function, (void *)thread_ids[total_threads-1]);
}


void quit_signal_handler(int signum)
{
	print_flag = OFF;
	printf("\nControl-c pressed (SIGINT), do you really");
	printf(" want to exit? [Y/n]");
	char choise;
	int i;
	scanf("%c",&choise);
	printf("\n");
	if ((choise == Y_SMALL) || (choise == Y_BIG)){
		end_program = clock();
		program_sec = (float)(end_program - start_program) / CLOCKS_PER_SEC;
		printf("Generated tasks: %d\t Excecuted tasks: %d\n", generated_tasks, excecuted_tasks);
		printf("Program excecution time: %lf\n", program_sec);
		printf("Terminating all working threads...\n");
		printf("Destroying thread attr...\n");
		pthread_attr_destroy(&attr);
		printf("Destroying worker mutex...\n");
		pthread_mutex_destroy(&worker_mutex);
		printf("Destroying worker condition variable...\n");
		pthread_cond_destroy(&worker_cv);
		printf("Thread exiting\n");
		printf("\nTask server finished.\n\n");

		exit(signum);
	}
	else {
		print_flag = ON;
	}
	return;
}
