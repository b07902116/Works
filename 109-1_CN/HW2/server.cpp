#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "opencv2/opencv.hpp"



#define PATHSIZE 256
#define BUFFERSIZE 65536

#define stop_connect -1
#define CMD_ls 1
#define CMD_put 2
#define CMD_get 3
#define CMD_play 4

#define FRAMELIMIT 30
#define BUFFERLIMIT 100


using namespace cv;


int check_cmd_type(const char *cmd){
	if (strcmp(cmd, "ls") == 0)
		return CMD_ls;
	else if (strcmp(cmd, "put") == 0)
		return CMD_put;
	else if (strcmp(cmd, "get") == 0)
		return CMD_get;
	else if (strcmp(cmd, "play") == 0)
		return CMD_play;
	// should not return 0
	else
		return 0;
}



int check_server_file_exist(int connFD, char *path){
	int filesize, pos, send_len;
	char response[BUFFERSIZE];
	bzero(response, BUFFERSIZE);
	struct stat st;
	if (access(path, F_OK) == -1){
		filesize = -1;
	}
	else{
		stat(path, &st);
		filesize = st.st_size;
	}
	sprintf(response, "%d", filesize);
	// Send filesize to client, msg size = BUFFERSIZE
	pos = 0;
	while (pos < BUFFERSIZE){
		send_len = send(connFD, &(response[pos]), BUFFERSIZE - pos, 0);
		pos += send_len;
	}
	return filesize;
}


void ls_func(int connFD){
	int filesize = 0, pos, total, send_len, send_size;
	char folder_path[PATHSIZE], filename[PATHSIZE], buf[BUFFERSIZE];
	bzero(filename, PATHSIZE);
	bzero(folder_path, PATHSIZE);
	bzero(buf, BUFFERSIZE);

	DIR *dptr;
	struct dirent *deptr;

	getcwd(folder_path, sizeof(folder_path));
	strcat(folder_path, "/server_folder");
	if ((dptr = opendir(folder_path)) == NULL){
		perror("ls opendir");
	}
	// count length of ls result
	while ((deptr = readdir(dptr)) != NULL){
		if (deptr->d_type == DT_DIR)
			continue;
		filesize += (strlen(deptr->d_name) + 1);
	}
	sprintf(buf, "%d", filesize);
	// send length of ls result
	pos = 0;
	while (pos < BUFFERSIZE){
		send_len = send(connFD, &(buf[pos]), BUFFERSIZE - pos, 0);
		pos += send_len;
	}
	// send ls result
	total = 0;
	seekdir(dptr, 0);
	while (total < filesize){
		if ((deptr = readdir(dptr)) == NULL){
			perror("deptr");
			break;
		}
		if (deptr->d_type == DT_DIR)
			continue;
		strncpy(filename, deptr->d_name, strlen(deptr->d_name));
		send_size = strlen(deptr->d_name) + 1;
		filename[send_size - 1] = '\n';

		// Assure send() can send send_size bytes data
		pos = 0;
		while (pos < send_size){
			send_len = send(connFD, &(filename[pos]), send_size - pos, 0);
			pos += send_len;
		}
		total += send_size;
	}
	fprintf(stderr, "finish ls\n");
	return;
}


int receive_filesize(int connFD){
	int filesize, total, recv_len;
	char buf[BUFFERSIZE];
	bzero(buf, BUFFERSIZE);
	total = 0;
	while (total < BUFFERSIZE){
		recv_len = recv(connFD, &(buf[total]), BUFFERSIZE - total, 0);
		total += recv_len;
	}
	sscanf(buf, "%d", &filesize);
	return filesize;
}


int is_mpg(int connFD, char *path, char *filename){
	int len, width, height, total, send_len;
	char extension[8] = {0}, buf[BUFFERSIZE];
	VideoCapture cap;
	bzero(buf, BUFFERSIZE);
	len = strlen(filename);
	strncpy(extension, &(filename[len - 4]), 4);
	if (strcmp(extension, ".mpg") != 0){
		width = 0;
		height = 0;
	}
	else{
		if (cap.open(path) != true){
			width = 0;
			height = 0;
		}
		else{
			width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
			height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
		}
	}
	// Send video resolution to client
	total = 0;
	sprintf(buf, "%d %d", width, height);
	while (total < BUFFERSIZE){
		send_len = send(connFD, &(buf[total]), BUFFERSIZE - total, 0);
		total += send_len;
	}
	if (width > 0 && height > 0)
		return 1;
	else
		return 0;
}




void create_server_folder(void){
	char folder_path[PATHSIZE];
	DIR *dptr;

	getcwd(folder_path, sizeof(folder_path));
	strcat(folder_path, "/server_folder");

	if ((dptr = opendir(folder_path)) == NULL){
		fprintf(stderr, "server_folder does not exist, now create...\n");
		mkdir(folder_path, 0777);
	}
	return;
}

int receive_file(int connFD, const char *path, int filesize, int nextpos){
	int fileFD, total = nextpos, pos, recv_len, recv_size, count = 0;
	char buf[BUFFERSIZE];
	bzero(buf, BUFFERSIZE);
	if (nextpos == 0)
		fileFD = open(path, O_WRONLY | O_TRUNC | O_CREAT);
	else
		fileFD = open(path, O_WRONLY | O_APPEND);

	while (count < BUFFERLIMIT){
		if (total >= filesize){
			close(fileFD);
			return 0;
		}
		recv_size = (filesize - total > BUFFERSIZE)? BUFFERSIZE : filesize - total;
		// Assure recv() can receive correct data
		pos = 0;
		while (pos < recv_size){
			recv_len = recv(connFD, &(buf[pos]), recv_size - pos, 0);
			pos += recv_len;
		}
		// Assure write() can write recv_size bytes into data
		pos = 0;
		while (pos < recv_size){
			recv_len = write(fileFD, &(buf[pos]), recv_size - pos);
			pos += recv_len;
		}
		count++;
		total += recv_size;
	}
	close(fileFD);
	return total;
}


int transmit_file(int connFD, const char *path, int filesize, int nextpos){
	int fileFD, total = nextpos, pos, send_len, send_size, count = 0;
	char buf[BUFFERSIZE];
	bzero(buf, BUFFERSIZE);
	fileFD = open(path, O_RDONLY);
	lseek(fileFD, nextpos, SEEK_SET);

	// loop to transmit
	while (count < BUFFERLIMIT){
		if (total >= filesize){
			close(fileFD);
			return 0;
		}
		send_size = (filesize - total > BUFFERSIZE)? BUFFERSIZE : filesize - total;

		// Assure read() can read send_size bytes into buf
		pos = 0;
		while (pos < send_size){
			send_len = read(fileFD, &(buf[pos]), send_size - pos);
			pos += send_len;
		}

		// Assure send() can send send_size bytes data
		pos = 0;
		while (pos < send_size){
			send_len = send(connFD, &(buf[pos]), send_size - pos, 0);
			pos += send_len;
		}
		count++;
		total += send_size;
	}
	close(fileFD);
	return total;
}


int streaming(int connFD, char *path){
	int width, height, framesize, framenum;
	VideoCapture cap;
	cap.open(path);
	width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	framenum = cap.get(CV_CAP_PROP_FRAME_COUNT);
	framesize = width * height * 3;

	int pos, send_len, recv_len, send_fps, recv_fps, count = 0;
	char buf[BUFFERSIZE];
	bzero(buf, BUFFERSIZE);
	uchar frame_buf[framesize];
	Mat frame;
	frame = Mat::zeros(height, width, CV_8UC3);
	if (!frame.isContinuous()){
		frame = frame.clone();
	}
	// Receive frame pos
	pos = 0;
	while (pos < BUFFERSIZE){
		recv_len = recv(connFD, &(buf[pos]), BUFFERSIZE - pos, 0);
		pos += recv_len;
	}
	sscanf(buf, "%d", &recv_fps);
	if (recv_fps == -1){
		cap.release();
		fprintf(stderr, "client stop streaming\n");
		return -1;
	}
	// Send frame pos
	if (recv_fps >= framenum){
		send_fps = -1;
	}
	else{
		if (recv_fps + FRAMELIMIT < framenum){
			send_fps = recv_fps + FRAMELIMIT;
		}
		else{
			send_fps = framenum;
		}
	}
	pos = 0;
	sprintf(buf, "%d", send_fps);
	while (pos < BUFFERSIZE){
		send_len = send(connFD, &(buf[pos]), BUFFERSIZE - pos, 0);
		pos += send_len;
	}
	if (send_fps == -1){
		cap.release();
		fprintf(stderr, "no more frame\n");
		return -1;
	}
	// Send frames that client request
	cap.set(CV_CAP_PROP_POS_FRAMES, recv_fps);
	while (recv_fps < send_fps){
		if (cap.read(frame)){
			memcpy(frame_buf, frame.data, framesize);
		}
		else{
			// If unable to read, use last OK frame and send to client
			fprintf(stderr, "cannot read %d-th frame\n", recv_fps);
		}
		pos = 0;
		while (pos < framesize){
			send_len = send(connFD, &(frame_buf[pos]), framesize - pos, 0);
			pos += send_len;
		}
		recv_fps++;
	}
	cap.release();
	return send_fps;
}




int command_handle(int connFD, int *filesize, char *filepath){
	int type, pos, recv_len;
	char cmd[8], buf[BUFFERSIZE], filename[PATHSIZE];
	bzero(buf, BUFFERSIZE);

	// Receive command from client, msg size = BUFFERSIZE
	// recv_len == 0 means client close connection
	pos = 0;
	while (pos < BUFFERSIZE){
		recv_len = recv(connFD, &(buf[pos]), BUFFERSIZE - pos, 0);
		pos += recv_len;
		if (recv_len == 0){
			return stop_connect;
		}
	}

	sscanf(buf, "%s%s", cmd, filename);
	type = check_cmd_type(cmd);
	switch (type){
		case CMD_ls :
			// do nothing
		break;

		case CMD_put :
			*filesize = receive_filesize(connFD);
			if (*filesize < 0)
				return 0;
			strcpy(filepath, "./server_folder/");
			strcat(filepath, filename);
		break;

		case CMD_get :
			strcpy(filepath, "./server_folder/");
			strcat(filepath, filename);
			*filesize = check_server_file_exist(connFD, filepath);
			if (*filesize < 0)
				return 0;
		break;

		case CMD_play :
			strcpy(filepath, "./server_folder/");
			strcat(filepath, filename);
			*filesize = check_server_file_exist(connFD, filepath);
			if (*filesize < 0)
				return 0;
			if (is_mpg(connFD, filepath, filename) == 0)
				return 0;
		break;
	}
	return type;
}




int main(int argc, char *argv[]){
	if (argc != 2){
		printf("server input format error\n");
		exit(0);
	}
	int server_port = atoi(argv[1]);

	// Create server's folder
	create_server_folder();

	int listenFD, connFD;
	socklen_t length;

	// Create socket
	listenFD = socket(AF_INET, SOCK_STREAM, 0);

	// Prepare sockaddr struct
	struct sockaddr_in server_address, client_address;
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;				// IPv4
	server_address.sin_port = htons(server_port);		// Port
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);	// IP

	// Bind socket and socketaddr
	bind(listenFD, (struct sockaddr *) &server_address, sizeof(server_address));

	// Listen
	listen(listenFD, 1);


	// select
	// Assume connFD will not exceed 64
	int maxFD = 64, type;
	int nextpos[64] = {0}, filesize[64] = {0};
	char filepath[64][PATHSIZE] = {0};
	struct timeval timeout;
	timeout.tv_sec = 0;
	timeout.tv_usec = 0;

	fd_set test_set, sock_set, get_set, put_set, play_set, main_set;
	FD_ZERO(&sock_set);
	FD_ZERO(&get_set);
	FD_ZERO(&put_set);
	FD_ZERO(&play_set);
	FD_ZERO(&main_set);
	FD_SET(listenFD, &sock_set);

	while (1){
		length = sizeof(client_address);
		
		// Check for new connection
		memcpy(&test_set, &sock_set, sizeof(main_set));
		if (select(maxFD, &test_set, NULL, NULL, &timeout) > 0){
			connFD = accept(listenFD, (struct sockaddr *) &client_address, &length);
			fprintf(stderr, "accept new connection\n");
			FD_SET(connFD, &main_set);
		}

		// Check for any command
		memcpy(&test_set, &main_set, sizeof(main_set));
		if (select(maxFD, &test_set, NULL, NULL, &timeout) > 0){

			// Find which socket the command comes from
			for (int i = 3; i < maxFD; i++){
				if (FD_ISSET(i, &test_set)){
					connFD = i;
					type = command_handle(connFD, &(filesize[connFD]), filepath[connFD]);
					switch (type){
						case CMD_ls :
							ls_func(connFD);
						break;

						case CMD_put :
							FD_SET(connFD, &put_set);
							FD_CLR(connFD, &main_set);
						break;

						case CMD_get :
							FD_SET(connFD, &get_set);
							FD_CLR(connFD, &main_set);
						break;

						case CMD_play :
							FD_SET(connFD, &play_set);
							FD_CLR(connFD, &main_set);
						break;

						case stop_connect :
							close(connFD);
							FD_CLR(connFD, &main_set);
							fprintf(stderr, "a client close connection\n");
						break;
					}
					
				}
			}
		}

		// Check for any put cmd
		memcpy(&test_set, &put_set, sizeof(put_set));
		if (select(maxFD, &test_set, NULL, NULL, &timeout) > 0){
			// Find which socket is putting
			for (int i = 3; i < maxFD; i++){
				if (FD_ISSET(i, &test_set)){
					connFD = i;
					nextpos[connFD] = receive_file(connFD, filepath[connFD], filesize[connFD], nextpos[connFD]);
					if (nextpos[connFD] == 0){
						fprintf(stderr, "finish put\n");
						FD_CLR(connFD, &put_set);
						FD_SET(connFD, &main_set);
					}
				}
			}
			
		}

		// Check for any get cmd
		memcpy(&test_set, &get_set, sizeof(get_set));
		if (select(maxFD, NULL, &test_set, NULL, &timeout) > 0){
			// Find which socket is getting
			for (int i = 3; i < maxFD; i++){
				if (FD_ISSET(i, &test_set)){
					connFD = i;
					nextpos[connFD] = transmit_file(connFD, filepath[connFD], filesize[connFD], nextpos[connFD]);
					if (nextpos[connFD] == 0){
						fprintf(stderr, "finish get\n");
						FD_CLR(connFD, &get_set);
						FD_SET(connFD, &main_set);
					}
				}
			}
			
		}

		
		// Check for any play cmd
		memcpy(&test_set, &play_set, sizeof(play_set));
		if (select(maxFD, &test_set, NULL, NULL, &timeout) > 0){
			// Find which socket is streaming
			for (int i = 3; i < maxFD; i++){
				if (FD_ISSET(i, &test_set)){
					connFD = i;

					if (streaming(connFD, filepath[connFD]) == -1){
						fprintf(stderr, "finish play\n");
						FD_CLR(connFD, &play_set);
						FD_SET(connFD, &main_set);
					}
				}
			}
			
		}
	

	}

	return 0;
}