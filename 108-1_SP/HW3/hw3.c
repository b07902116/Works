#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include "scheduler.h"

#define SIGUSR3 SIGWINCH

int P, Q, task, loop_times;
int mutex = 0;
int inqueue[5] = {0};
int idx;
char arr[10000];
jmp_buf SCHEDULER;
FCB blocks[5];
FCB_ptr Head, Current;


void build_circular_list(void){
	Head = NULL;
	Current = &(blocks[4]);
	for (int i = 1; i <= 4; i++){
		int j = i % 4 + 1;
		int k = i - 1;
		if (i == 1)
			k = 4;
		blocks[i].Next = &(blocks[j]);
		blocks[i].Previous = &(blocks[k]);
		blocks[i].Name = i;
	}
	return;
}

void handler_1(int signum){
	//fprintf(stderr, "in handler_1\n");
	sigset_t new;
	sigemptyset(&new);
	sigaddset(&new, SIGUSR1);
	sigaddset(&new, SIGUSR2);
	sigaddset(&new, SIGUSR3);
	sigprocmask(SIG_SETMASK, &new, 0);
	printf("ACK\n");
	fflush(stdout);
	longjmp(SCHEDULER, 1);
}

void handler_2(int signum){
	//fprintf(stderr, "in handler_2\n");
	sigset_t new;
	sigemptyset(&new);
	sigaddset(&new, SIGUSR1);
	sigaddset(&new, SIGUSR2);
	sigaddset(&new, SIGUSR3);
	sigprocmask(SIG_SETMASK, &new, 0);
	printf("ACK\n");
	fflush(stdout);
	longjmp(SCHEDULER, 1);
}

void handler_3(int signum){
	//fprintf(stderr, "in handler_3\n");
	for (int i = 1; i < 5; i++){
		if (inqueue[i])
			printf("%d ", i);
	}
	printf("\n");
	fflush(stdout);
	sigset_t new;
	sigemptyset(&new);
	sigaddset(&new, SIGUSR1);
	sigaddset(&new, SIGUSR2);
	sigaddset(&new, SIGUSR3);
	sigprocmask(SIG_SETMASK, &new, 0);
	Current = Current->Previous;//to jump to original function
	longjmp(SCHEDULER, 1);
}


void funct_1(void){
	int i, j;
	signal(SIGUSR1, handler_1);
	signal(SIGUSR2, handler_2);
	signal(SIGUSR3, handler_3);
	if (setjmp(blocks[1].Environment) == 0){
		funct_5(2);
	}
	else{
		static int count = 0;
		if (mutex == 0 || mutex == 1){
			mutex = 1;
			inqueue[1] = 0;
			for (j = 1; j <= P; j++){
				if (count == P){
					mutex = 0;
					longjmp(SCHEDULER, -2);
				}
				sigset_t new, emp, pend;
				sigemptyset(&new);
				sigemptyset(&emp);
				sigemptyset(&pend);
				sigaddset(&new, SIGUSR1);
				sigaddset(&new, SIGUSR2);
				sigaddset(&new, SIGUSR3);
				sigprocmask(SIG_SETMASK, &new, 0);
				for (i = 1; i <= Q; i++){
					sleep(1);
					arr[idx++] = '1';
				}
				count++;
				if (j == loop_times){
					mutex = 0;
					longjmp(SCHEDULER, 1);
				}

				sigpending(&pend);
				if (sigismember(&pend, SIGUSR2))
					mutex = 0;
				sigprocmask(SIG_SETMASK, &emp, 0);
				//go to signal handler
			}
			mutex = 0;
			longjmp(SCHEDULER, -2);
		}
		else{
			inqueue[1] = 1;
			longjmp(SCHEDULER, 1);
		}
	}
}

void funct_2(void){
	int i, j;
	signal(SIGUSR1, handler_1);
	signal(SIGUSR2, handler_2);
	signal(SIGUSR3, handler_3);
	if (setjmp(blocks[2].Environment) == 0){
		funct_5(3);
	}
	else{
		static int count = 0;
		if (mutex == 0 || mutex == 2){
			mutex = 2;
			inqueue[2] = 0;
			for (j = 1; j <= P; j++){
				if (count == P){
					mutex = 0;
					longjmp(SCHEDULER, -2);
				}
				sigset_t new, emp, pend;
				sigemptyset(&new);
				sigemptyset(&emp);
				sigemptyset(&pend);
				sigaddset(&new, SIGUSR1);
				sigaddset(&new, SIGUSR2);
				sigaddset(&new, SIGUSR3);
				sigprocmask(SIG_SETMASK, &new, 0);
				for (i = 1; i <= Q; i++){
					sleep(1);
					arr[idx++] = '2';
				}
				count++;
				if (j == loop_times){
					mutex = 0;
					longjmp(SCHEDULER, 1);
				}

				sigpending(&pend);
				if (sigismember(&pend, SIGUSR2))
					mutex = 0;
				sigprocmask(SIG_SETMASK, &emp, 0);
				//go to signal handler
			}
			mutex = 0;
			longjmp(SCHEDULER, -2);
		}
		else{
			inqueue[2] = 1;
			longjmp(SCHEDULER, 1);
		}
	}
}

void funct_3(void){
	int i, j;
	signal(SIGUSR1, handler_1);
	signal(SIGUSR2, handler_2);
	signal(SIGUSR3, handler_3);
	if (setjmp(blocks[3].Environment) == 0){
		funct_5(4);
	}
	else{
		static int count = 0;
		if (mutex == 0 || mutex == 3){
			mutex = 3;
			inqueue[3] = 0;
			for (j = 1; j <= P; j++){
				if (count == P){
					mutex = 0;
					longjmp(SCHEDULER, -2);
				}
				sigset_t new, emp, pend;
				sigemptyset(&new);
				sigemptyset(&emp);
				sigemptyset(&pend);
				sigaddset(&new, SIGUSR1);
				sigaddset(&new, SIGUSR2);
				sigaddset(&new, SIGUSR3);
				sigprocmask(SIG_BLOCK, &new, 0);
				for (i = 1; i <= Q; i++){
					sleep(1);
					arr[idx++] = '3';
				}
				count++;
				if (j == loop_times){
					mutex = 0;
					longjmp(SCHEDULER, 1);
				}

				sigpending(&pend);
				if (sigismember(&pend, SIGUSR2))
					mutex = 0;
				sigprocmask(SIG_SETMASK, &emp, 0);
				//go to signal handler
			}
			mutex = 0;
			longjmp(SCHEDULER, -2);
		}
		else{
			inqueue[3] = 1;
			longjmp(SCHEDULER, 1);
		}
	}
}

void funct_4(void){
	int i, j;
	signal(SIGUSR1, handler_1);
	signal(SIGUSR2, handler_2);
	signal(SIGUSR3, handler_3);
	if (setjmp(blocks[4].Environment) == 0){
		build_circular_list();
		longjmp(blocks[0].Environment, 1);
	}
	else{
		static int count = 0;
		if (mutex == 0 || mutex == 4){
			mutex = 4;
			inqueue[4] = 0;
			for (j = 1; j <= P; j++){
				if (count == P){
					mutex = 0;
					longjmp(SCHEDULER, -2);
				}
				sigset_t new, emp, pend;
				sigemptyset(&new);
				sigemptyset(&emp);
				sigemptyset(&pend);
				sigaddset(&new, SIGUSR1);
				sigaddset(&new, SIGUSR2);
				sigaddset(&new, SIGUSR3);
				sigprocmask(SIG_BLOCK, &new, 0);
				for (i = 1; i <= Q; i++){
					sleep(1);
					arr[idx++] = '4';
				}
				count++;
				if (j == loop_times){
					mutex = 0;
					longjmp(SCHEDULER, 1);
				}

				sigpending(&pend);
				if (sigismember(&pend, SIGUSR2))
					mutex = 0;
				sigprocmask(SIG_SETMASK, &emp, 0);
				//go to signal handler
			}
			mutex = 0;
			longjmp(SCHEDULER, -2);
		}
		else{
			inqueue[4] = 1;
			longjmp(SCHEDULER, 1);
		}
	}
}

void funct_5(int name){//dummy
	int a[10000];
	switch (name){
		case 1 :
		funct_1();
		break;
		case 2 :
		funct_2();
		break;
		case 3 :
		funct_3();
		break;
		case 4 :
		funct_4();
		break;
		default :
		exit(0);
		break;
	}
	return;
}


int main(int argc, char *argv[]){
	P = atoi(argv[1]);
	Q = atoi(argv[2]);
	task = atoi(argv[3]);
	loop_times = atoi(argv[4]);
	if (setjmp(blocks[0].Environment) == 0){
		funct_5(1);
	}
	else{
		Scheduler();
	}
	return 0;
}