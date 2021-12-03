#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/times.h>


void print_rank(int player_num, int score[]){
	int max, times, rank = 1;
	int output[player_num + 1];
	while (rank <= player_num){
		max = 0;
		times = 0;
		for (int j = 1; j <= player_num; j++){
			if (score[j] > max){
				max = score[j];
				times = 1;
			}
			else if (score[j] == max){
				times++;
			}
		}
		for (int j = 1; j <= player_num; j++){
			if (score[j] == max){
				score[j] = -1;
				output[j] = rank;
			}
		}
		rank += times;
	}
	for (int i = 1; i <= player_num; i++){
		printf("%d %d\n", i, output[i]);
	}
	return;
}

void add_score(int rsp_id[], int rsp_rank[], int score[]){
	for (int i = 0; i < 8; i++)
		score[rsp_id[i]] += (8 - rsp_rank[i]);
	return;
}

int next_comptition(int player_num, int *competition){
	int count = 0;
	while (competition[7 - count] == player_num - count){
		count++;
		if (count == 8)
			return 0;
	}
	competition[7 - count]++;
	for (int i = count - 1; i >= 0; i--){
		competition[7 - i] = competition[7 - i - 1] + 1;
	}
}


int main(int argc, char *argv[]){
	int host_num = atoi(argv[1]), player_num = atoi(argv[2]);
	int score[player_num + 1], player_id[player_num + 1];
	int host_index[host_num + 1], host_pid[host_num + 1], host_rand[host_num + 1], write_fifo_fd[host_num + 1];
	int competition[8] = {1, 2, 3, 4, 5, 6, 7, 8};
	pid_t pid;
	FILE *FIFO_PTR;
	char fifoname[16], host_arv[4][16];
	for (int i = 0; i <= player_num; i++){
		score[i] = 0;
	}
	mkfifo("Host.FIFO", 0666);
	FIFO_PTR = fopen("Host.FIFO", "r+");
	//bidding_system first fork host_num root_host
	for (int i = 1; i <= host_num; i++){
		sprintf(fifoname, "Host%d.FIFO", i);
		mkfifo(fifoname, 0666);
		host_rand[i] = rand() % 65535;
		pid = fork();
		if (pid == 0){//child as root_host
			sprintf(host_arv[0], "%d", i);//host_num
			sprintf(host_arv[1], "%d", host_rand[i]);//random value
			sprintf(host_arv[2], "%d", 0);//depth
			execl("host", "host", host_arv[0], host_arv[1], host_arv[2], NULL);//execute a root_host
		}
		else{//parent
			host_index[i] = i;
			host_pid[i] = pid;
			write_fifo_fd[i] = open(fifoname, O_WRONLY);
		}
	}
	//bidding_system assign competition to idle root_host
	int count = 1, yet = 1;
	char buffer[256];
	//assign to each host_root
	while (count <= host_num && yet){
		sprintf(buffer, "%d %d %d %d %d %d %d %d\n\0", competition[0], competition[1], competition[2], 
				competition[3], competition[4], competition[5], competition[6], competition[7]);
		write(write_fifo_fd[count], buffer, strlen(buffer));
		fsync(write_fifo_fd[count]);
		yet = next_comptition(player_num, competition);
		count++;
	}
	//too many root_host
	for (int i = count; i <= host_num; i++){
		sprintf(buffer, "-1 -1 -1 -1 -1 -1 -1 -1\n\0");
		write(write_fifo_fd[i], buffer, strlen(buffer));
		fsync(write_fifo_fd[i]);
		close(write_fifo_fd[i]);
		sprintf(fifoname, "Host%d.FIFO\0", i);
		unlink(fifoname);
		wait(NULL);
	}
	//check if there is idle root_host
	int free, randval, line;
	int rsp_id[8], rsp_rank[8];
	while (yet){
		line = 0;
		while (line < 9){
			fgets(buffer, 256, FIFO_PTR);
			if (line == 0)
				sscanf(buffer, "%d", &randval);
			else
				sscanf(buffer, "%d %d", &(rsp_id[line - 1]), &(rsp_rank[line - 1]));
			line++;
		}
		for (int i = 1; i <= host_num; i++)
			if (randval == host_rand[i])
				free = i;
		add_score(rsp_id, rsp_rank, score);
		sprintf(buffer, "%d %d %d %d %d %d %d %d\n\0", competition[0], competition[1], competition[2], 
				competition[3], competition[4], competition[5], competition[6], competition[7]);
		write(write_fifo_fd[free], buffer, strlen(buffer));
		fsync(write_fifo_fd[free]);
		yet = next_comptition(player_num, competition);
	}
	//read last response of each root_host
	int number;
	for (int i = 1; i <= count - 1; i++){
		line = 0;
		while (line < 9){
			fgets(buffer, 256, FIFO_PTR);
			if (line == 0)
				sscanf(buffer, "%d", &randval);
			else
				sscanf(buffer, "%d %d", &(rsp_id[line - 1]), &(rsp_rank[line - 1]));
			line++;
		}
		for (int j = 1; j <= host_num; j++)
			if (randval == host_rand[j])
				free = j;
		add_score(rsp_id, rsp_rank, score);
		sprintf(buffer, "-1 -1 -1 -1 -1 -1 -1 -1\n\0");
		write(write_fifo_fd[free], buffer, strlen(buffer));
		fsync(write_fifo_fd[free]);
		//close(write_fifo_fd[free]);
		wait(NULL);
		sprintf(fifoname, "Host%d.FIFO\0", free);
		unlink(fifoname);
	}
	unlink("Host.FIFO");
	print_rank(player_num, score);
	return 0;
}