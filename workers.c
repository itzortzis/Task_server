#include <stdio.h>
#include <stdlib.h>
#include "workers.h"
#include "task_server.h"
#include <math.h>
#include <string.h>


int sum = 0;
void print_work()
{
	printf("Yeaaaaaaaaaaahhhhhhhh!\n");
}


// TYPE 1 and TYPE 3
void prime_numbers(int n) {
	int i, j, counter = 0, flag, primes;

	for (i=1; i<=n; i++) {
		flag = 0;
		if (i > 1) {
			for (j=1; j<=i/2; j++) {
				if ((i%j == 0) && (j != i) && (j != 1)) {
					flag = 1;
				}
			}
		}
		if (flag == 1) {
			counter++;
		}
		if ((flag == 0) && (i>1)) {
			//printf("%d, ", i);
			sum = sum + i;
		}
	}

	primes = n-counter-1;
	//printf("\nN:%d\t Primes:%d\n", n, primes);
	//printf("Sum: %d\n", sum);
}


// TYPE 2
void memory_copy(int A, int B, int N) 
{
	long int sub = abs(&A - &B);
	//printf("Sub: %ld\n", sub);
	if ((long int)N <= sub) {
		memcpy(&A, &B, N);
		//printf("Copy completed successfully!!!\n");
	}
	else {
		//printf("No copy!!!\n");
	}
}
