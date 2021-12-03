#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>


void decide_rank(int win_id, int player_id[], int rank[]){
	for (int i = 0; i < 8; i++){
		if (player_id[i] == win_id)
			rank[i] = 1;
		else
			rank[i] = 2;
	}
	return;
}

int get_winner(int fda, int fdb){
	char buffer[256];
	int cmp_id[2], cmp_money[2], win_id;
	read(fda, buffer, 256);
	sscanf(buffer, "%d%d", &cmp_id[0], &cmp_money[0]);
	read(fdb, buffer, 256);
	sscanf(buffer, "%d%d", &cmp_id[1], &cmp_money[1]);
	win_id = cmp_money[0] > cmp_money[1]? cmp_id[0] : cmp_id[1];
	return win_id;
}


int main(int argc, char *argv[]){
	int host_id = atoi(argv[1]), random_key = atoi(argv[2]), depth = atoi(argv[3]);
	int player_id[8], child_pipe[4][2], win_id;
	pid_t pid[2];
	char buffer[256];
	if (depth == 0){//root_host
		int rank[8], fifo_write_fd;
		for (int i = 0; i < 4; i++)
			pipe(child_pipe[i]);
		//child_pipe[0, 1] for first child_host
		//child_pipe[2, 3] for second child_host
		//child_pipe[0][1] : parent write to child
		//child_pipe[1][0] : parent read from child
		//child_pipe[2][1] : parent write to child
		//child_pipe[3][0] : parent read from child
		pid[0] = fork();
		if (pid[0] == 0){//first child_host
			dup2(child_pipe[0][0], STDIN_FILENO);
			dup2(child_pipe[1][1], STDOUT_FILENO);
			execl("host", "host", argv[1], argv[2], "1", NULL);
		}
		else{
			pid[1] = fork();
			if (pid[1] == 0){//second child_host
				dup2(child_pipe[2][0], STDIN_FILENO);
				dup2(child_pipe[3][1], STDOUT_FILENO);
				execl("host", "host", argv[1], argv[2], "1", NULL);
			}
			else{//root_host
				FILE *RD_FIFO;
				sprintf(buffer, "Host%d.FIFO\0", host_id);
				RD_FIFO = fopen(buffer, "r+");
				fifo_write_fd = open("Host.FIFO", O_WRONLY);
				fgets(buffer, 256, RD_FIFO);
				sscanf(buffer, "%d%d%d%d%d%d%d%d", &player_id[0], &player_id[1], &player_id[2], 
						&player_id[3], &player_id[4], &player_id[5], &player_id[6], &player_id[7]);
				while (player_id[0] != -1){
					//first round
					for (int i = 0; i < 2; i++){
						sprintf(buffer, "%d %d %d %d\n\0", player_id[4 * i], player_id[4 * i + 1], player_id[4 * i + 2], player_id[4 * i + 3]);
						write(child_pipe[2 * i][1], buffer, strlen(buffer));
						fsync(child_pipe[2 * i][1]);
					}
					win_id = get_winner(child_pipe[1][0], child_pipe[3][0]);
					//following 9 rounds
					for (int times = 0; times < 9; times++){
						sprintf(buffer, "%d\n\0", win_id);
						for (int i = 0; i < 2; i++){
							write(child_pipe[i * 2][1], buffer, strlen(buffer));
							fsync(child_pipe[i * 2][1]);
						}
						win_id = get_winner(child_pipe[1][0], child_pipe[3][0]);
					}
					//after 10 round
					decide_rank(win_id, player_id, rank);
					sprintf(buffer, "%d\n%d %d\n%d %d\n%d %d\n%d %d\n%d %d\n%d %d\n%d %d\n%d %d\n\0", random_key, player_id[0], rank[0], 
							player_id[1], rank[1], player_id[2], rank[2], player_id[3], rank[3], player_id[4], rank[4], 
							player_id[5], rank[5], player_id[6], rank[6], player_id[7], rank[7]);
					write(fifo_write_fd, buffer, strlen(buffer));
					fsync(fifo_write_fd);
					//one competition end and read new player
					fgets(buffer, 256, RD_FIFO);
					sscanf(buffer, "%d%d%d%d%d%d%d%d", &player_id[0], &player_id[1], &player_id[2], 
							&player_id[3], &player_id[4], &player_id[5], &player_id[6], &player_id[7]);
				}
				//receive -1 -1 ... -1\n
				for (int i = 0; i < 2; i++){
					sprintf(buffer, "-1 -1 -1 -1\n\0");
					write(child_pipe[2 * i][1], buffer, strlen(buffer));
					fsync(child_pipe[2 * i][1]);
					wait(NULL);
				}
				_exit(0);
			}
		}
	}
	else if (depth == 1){//child_host
		//STDIN read from parent, STDOUT write to parent
		for (int i = 0; i < 4; i++)
			pipe(child_pipe[i]);
		pid[0] = fork();
		if (pid[0] == 0){//first leaf_host
			dup2(child_pipe[0][0], STDIN_FILENO);
			dup2(child_pipe[1][1], STDOUT_FILENO);
			execl("host", "host", argv[1], argv[2], "2", NULL);
		}
		else{
			pid[1] = fork();
			if (pid[1] == 0){//second leaf_host
				dup2(child_pipe[2][0], STDIN_FILENO);
				dup2(child_pipe[3][1], STDOUT_FILENO);
				execl("host", "host", argv[1], argv[2], "2", NULL);
			}
			else{//child_host
				scanf("%d%d%d%d", &player_id[0], &player_id[1], &player_id[2], &player_id[3]);
				while (player_id[0] != -1){
					//first round
					for (int i = 0; i < 2; i++){
						sprintf(buffer, "%d %d\n\0", player_id[2 * i], player_id[2 * i + 1]);
						write(child_pipe[2 * i][1], buffer, strlen(buffer));
						fsync(child_pipe[2 * i][1]);
					}
					win_id = get_winner(child_pipe[1][0], child_pipe[3][0]);
					sprintf(buffer, "%d %d\n\0", win_id, win_id * 100);
					write(STDOUT_FILENO, buffer, strlen(buffer));
					fsync(STDOUT_FILENO);
					//following 9 rounds
					for (int times = 0; times < 9; times++){
						//read real winner from root_host and pass down
						scanf("%d", &win_id);
						for (int i = 0; i < 2; i++){
							sprintf(buffer, "%d\n\0", win_id);
							write(child_pipe[2 * i][1], buffer, strlen(buffer));
							fsync(child_pipe[2 * i][1]);
						}
						win_id = get_winner(child_pipe[1][0], child_pipe[3][0]);
						//write possible winner to root_host
						sprintf(buffer, "%d %d\n\0", win_id, win_id * 100);
						write(STDOUT_FILENO, buffer, strlen(buffer));
						fsync(STDOUT_FILENO);
					}
					//after 10 round
					//one competition end and read new player
					scanf("%d%d%d%d", &player_id[0], &player_id[1], &player_id[2], &player_id[3]);
				}
				//receive -1 -1 -1 -1\n
				sprintf(buffer, "-1 -1\n\0");
				for (int i = 0; i < 2; i++){
					write(child_pipe[2 * i][1], buffer, strlen(buffer));
					fsync(child_pipe[2 * i][1]);
					wait(NULL);
				}
				_exit(0);
			}
		}
	}
	else if (depth == 2){//leaf_host
		//STDIN read from parent(child_host), STDOUT write to parent(child_host)
		for (int i = 0; i < 4; i++)
			pipe(child_pipe[i]);
		scanf("%d%d", &player_id[0], &player_id[1]);
		while (player_id[0] != -1){
			pid[0] = fork();
			if (pid[0] == 0){//first player
				dup2(child_pipe[0][0], STDIN_FILENO);
				dup2(child_pipe[1][1], STDOUT_FILENO);
				sprintf(buffer, "%d\n\0", player_id[0]);
				execl("player", "player", buffer, NULL);
			}
			else{
				pid[1] = fork();
				if (pid[1] == 0){//second player
					dup2(child_pipe[2][0], STDIN_FILENO);
					dup2(child_pipe[3][1], STDOUT_FILENO);
					sprintf(buffer, "%d\n\0", player_id[1]);
					execl("player", "player", buffer, NULL);
				}
				else{//leaf_host
					//first round
					win_id = get_winner(child_pipe[1][0], child_pipe[3][0]);
					sprintf(buffer, "%d %d\n\0", win_id, win_id * 100);
					write(STDOUT_FILENO, buffer, strlen(buffer));
					fsync(STDOUT_FILENO);
					for (int times = 0; times < 9; times++){
						//read real winner from child_host and pass down
						scanf("%d", &win_id);
						for (int i = 0; i < 2; i++){
							sprintf(buffer, "%d\n\0", win_id);
							write(child_pipe[2 * i][1], buffer, strlen(buffer));
							fsync(child_pipe[2 * i][1]);
						}
						win_id = get_winner(child_pipe[1][0], child_pipe[3][0]);
						sprintf(buffer, "%d %d\n\0", win_id, win_id * 100);
						write(STDOUT_FILENO, buffer, strlen(buffer));
						fsync(STDOUT_FILENO);
					}
					for (int i = 0; i < 2; i++)
						wait(NULL);
					//two players die
					//after 10 rounds
					scanf("%d%d", &player_id[0], &player_id[1]);
				}
			}
		}
		_exit(0);
	}
	return 0;
}