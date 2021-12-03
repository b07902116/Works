#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SIGUSR3 SIGWINCH


int main(void){
	int P, Q, R;
	scanf("%d%d%d", &P, &Q, &R);
	int pipe_fd[2];
	pipe(pipe_fd);
	pid_t pid = fork();
	if (pid == 0){//child run hw3
		char argv[4][16];
		sprintf(argv[0], "%d", P);
		sprintf(argv[1], "%d", Q);
		sprintf(argv[2], "%d", 3);
		sprintf(argv[3], "%d", 0);
		dup2(pipe_fd[1], STDOUT_FILENO);
		execl("hw3", "hw3", argv[0], argv[1], argv[2], argv[3], NULL);
	}
	else{
		int sig[R], len;
		char buf[256];
		for (int i = 0; i < R; i++){
			scanf("%d", &sig[i]);
		}
		for (int i = 0; i < R; i++){
			//fprintf(stderr, "sig = %d\n", sig[i]);
			sleep(5);
			if (sig[i] == 1)
				kill(pid, SIGUSR1);
			else if (sig[i] == 2)
				kill(pid, SIGUSR2);
			else
				kill(pid, SIGUSR3);
			len = read(pipe_fd[0], buf, 256);//read ACK or queue
			buf[len] = '\0';
			if (sig[i] == 3)
				printf("%s", buf);
		}
		len = read(pipe_fd[0], buf, 256);//read arr
		buf[len] = '\0';
		wait(NULL);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
		printf("%s", buf);
		return 0;
	}
}