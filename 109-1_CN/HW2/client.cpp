#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <fcntl.h>
#include "opencv2/opencv.hpp"




#define PATHSIZE 256
#define BUFFERSIZE 65536

#define FRAMELIMIT 30

#define CMD_not_found -2
#define CMD_format_error -1
#define CMD_exit 0
#define CMD_ls 1
#define CMD_put 2
#define CMD_get 3
#define CMD_play 4


using namespace cv;

int check_cmd_type(char *buf, char *cmd, char *filename){
	int argc = 0, retval;
	char other[BUFFERSIZE];
	argc = sscanf(buf, "%s%s%s", cmd, filename, other);
	if (strcmp(cmd, "ls") == 0){
		if (argc != 1)		retval = CMD_format_error;
		else				retval = CMD_ls;
	}
	else if ((strcmp(cmd, "put") == 0)){
		if (argc != 2)		retval = CMD_format_error;
		else				retval = CMD_put;
	}
	else if (strcmp(cmd, "get") == 0){
		if (argc != 2)		retval = CMD_format_error;
		else				retval = CMD_get;
	}
	else if (strcmp(cmd, "play") == 0){
		if (argc != 2)		retval = CMD_format_error;
		else				retval = CMD_play;
	}
	else if (strcmp(cmd, "exit") == 0){
		retval = CMD_exit;
	}
	else{
		retval = CMD_not_found;
	}
	return retval;
}


int check_client_file_exist(int connFD, char *path){
	int filesize;
	struct stat st;
	if (access(path, F_OK) == -1){
		filesize = -1;
	}
	else{
		stat(path, &st);
		filesize = st.st_size;
	}
	return filesize;
}


int check_server_file_exist(int connFD, char *buf){
	int filesize, pos, send_len, recv_len;
	char response[BUFFERSIZE];
	bzero(response, BUFFERSIZE);
	// Send command to server, msg size = BUFFERSIZE
	pos = 0;
	while (pos < BUFFERSIZE){
		send_len = send(connFD, &(buf[pos]), BUFFERSIZE - pos, 0);
		pos += send_len;
	}

	// Receive filesize from server, msg size = BUFFERSIZE
	pos = 0;
	while (pos < BUFFERSIZE){
		recv_len = recv(connFD, &(response[pos]), BUFFERSIZE - pos, 0);
		pos += recv_len;
	}
	sscanf(response, "%d", &filesize);
	return filesize;
}

int ismpg(int connFD, int *width, int *height){
	int pos, recv_len;
	char buf[BUFFERSIZE];
	// Receive resolution of the video
	pos = 0;
	while (pos < BUFFERSIZE){
		recv_len = recv(connFD, &(buf[pos]), BUFFERSIZE - pos, 0);
		pos += recv_len;
	}
	sscanf(buf, "%d %d", width, height);
	if (*width > 0 && *height > 0)
		return 1;
	else
		return 0;
}


void create_client_folder(int *folder_index){
	char folder_path[PATHSIZE] = {0};
	DIR *dptr;

	do{
		(*folder_index) ++;
		sprintf(folder_path, "./client_folder_%d", *folder_index);
	} while ((dptr = opendir(folder_path)) != NULL);
	fprintf(stderr, "client_folder does not exist, now create...\n");
	
	if (mkdir(folder_path, 0777) != 0)
		perror("mkdir");
	return;
}


void ls_func(int connFD){
	int filesize = 0, pos, total, send_len, recv_len, recv_size;
	char buf[BUFFERSIZE];
	bzero(buf, BUFFERSIZE);
	// send command to server
	sprintf(buf, "ls");
	pos = 0;
	while (pos < BUFFERSIZE){
		send_len = send(connFD, &(buf[pos]), BUFFERSIZE - pos, 0);
		pos += send_len;
	}
	// receive length of ls result
	pos = 0;
	while (pos < BUFFERSIZE){
		recv_len = recv(connFD, &(buf[pos]), BUFFERSIZE - pos, 0);
		pos += recv_len;
	}
	sscanf(buf, "%d", &filesize);

	// receive ls result
	fprintf(stderr, "------------------\n");
	total = 0;
	while (total < filesize){
		pos = 0;
		recv_len = recv(connFD, buf, BUFFERSIZE, 0);
		while (pos < recv_len){
			putchar(buf[pos++]);
		}
		total += recv_len;
	}
	fprintf(stderr, "------------------\n");
	return;
}



void receive_file(int connFD, const char *path, int filesize){
	int fileFD, total = 0, pos, recv_len, recv_size;
	char buf[BUFFERSIZE];
	bzero(buf, BUFFERSIZE);
	fileFD = open(path, O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
	if (fileFD < 0)
		perror("open in receive_file");
	fprintf(stderr, "start to receive file, filesize = %d\n", filesize);

	while (total < filesize){
		recv_size = (filesize - total > BUFFERSIZE)? BUFFERSIZE : filesize - total;

		// Assure recv() can receive recv_size bytes data
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
		total += recv_size;
	}
	close(fileFD);
	fprintf(stderr, "receive finish\n");
	return;
}

void transmit_file(int connFD, const char *path, int filesize){
	int fileFD, total = 0, pos, send_len, send_size;
	char buf[BUFFERSIZE];
	bzero(buf, BUFFERSIZE);
	fileFD = open(path, O_RDONLY);
	fprintf(stderr, "start to send file, filesize = %d\n", filesize);

	while (total < filesize){
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
		total += send_size;
	}
	close(fileFD);
	fprintf(stderr, "send finish\n");
	return;
}

void streaming(int connFD, int width, int height, int filesize){
	Mat frame;
	frame = Mat::zeros(height, width, CV_8UC3);
	if (!frame.isContinuous()){
		frame = frame.clone();
	}
	int framesize = width * height * 3;
	int pos, recv_len, send_len, count = 0, send_fps = 0, recv_fps = 0;
	char c;
	char buf[BUFFERSIZE];
	bzero(buf, BUFFERSIZE);
	uchar *ptr = frame.data;
	uchar frame_buf[framesize];
	while (1){
		// If client need more frame, request server FRAMELIMIT frames
		if (send_fps == recv_fps){
			// Send frame pos
			pos = 0;
			sprintf(buf, "%d", send_fps);
			while (pos < BUFFERSIZE){
				send_len = send(connFD, &(buf[pos]), BUFFERSIZE - pos, 0);
				pos += send_len;
			}

			// Receive frame pos
			pos = 0;
			while (pos < BUFFERSIZE){
				recv_len = recv(connFD, &(buf[pos]), BUFFERSIZE - pos, 0);
				pos += recv_len;
			}
			sscanf(buf, "%d", &recv_fps);
			if (recv_fps == -1){
				fprintf(stderr, "video end\n");
				break;
			}
		}
		// Receive a frame
		pos = 0;
		while (pos < framesize){
			recv_len = recv(connFD, &(frame_buf[pos]), framesize - pos, 0);
			pos += recv_len;
		}
		send_fps++;

		memcpy(ptr, frame_buf, framesize);
		imshow("Video", frame);
		c = (char) waitKey(33.3333);
		if (c == 27){
			// Tell server to stop
			pos = 0;
			strcpy(buf, "-1\0");
			while (pos < BUFFERSIZE){
				send_len = send(connFD, &(buf[pos]), BUFFERSIZE - pos, 0);
				pos += send_len;
			}
			// Receive the remaining frame to flush socket
			while (send_fps < recv_fps){
				pos = 0;
				while (pos < framesize){
					recv_len = recv(connFD, &(frame_buf[pos]), framesize - pos, 0);
					pos += recv_len;
				}
				send_fps++;
			}
			break;
		}
	}
	fprintf(stderr, "client stop streaming\n");
	destroyAllWindows();
	return;
}



void send_cmd_and_size(int connFD, char *buf, int filesize){
	int total = 0, send_len;
	char msg[BUFFERSIZE];
	bzero(msg, BUFFERSIZE);
	sprintf(msg, "%d", filesize);
	// Send command to server, msg size = BUFFERSIZE
	total = 0;
	while (total < BUFFERSIZE){
		send_len = send(connFD, &(buf[total]), BUFFERSIZE - total, 0);
		total += send_len;
	}
	// Send filesize to server, msg size = BUFFERSIZE
	total = 0;
	while (total < BUFFERSIZE){
		send_len = send(connFD, &(msg[total]), BUFFERSIZE - total, 0);
		total += send_len;
	}
	return;
}

void get_ip_port(char *input, char *ip, int *port){
	int pos = -1, len;
	len = strlen(input);
	for (int i = 0; i < len; i++){
		if (input[i] == ':'){
			pos = i;
			break;
		}
	}
	strncpy(ip, input, pos);
	*port = atoi(&(input[pos + 1]));
	return;
}


int main(int argc, char *argv[]){
	if (argc != 2){
		printf("client input format error\n");
		exit(0);
	}
	int server_port;
	char server_ip[32];
	get_ip_port(argv[1], server_ip, &server_port);
	fprintf(stderr, "ip = %s\nport = %d\n", server_ip, server_port);


	int sockFD;
	struct sockaddr_in server_address;
	// Create socket
	sockFD = socket(AF_INET, SOCK_STREAM, 0);

	// Prepare socketaddr struct
	bzero(&server_address, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(server_port);
	inet_pton(AF_INET, server_ip, &server_address.sin_addr);

	// Connect
	if (connect(sockFD, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
		perror("connect");
		exit(0);
	}


	int type, filesize, width, height, folder_index = 0;
	char writebuf[BUFFERSIZE];
	char cmd[PATHSIZE], path[PATHSIZE], filename[PATHSIZE];
	bzero(path, PATHSIZE);
	bzero(writebuf, BUFFERSIZE);

	create_client_folder(&folder_index);
	fprintf(stderr, "folder_index is %d\n", folder_index);

	while (1){
		fprintf(stderr, "type some message to server\n");
		// Determine command type
		fgets(writebuf, BUFFERSIZE, stdin);
		type = check_cmd_type(writebuf, cmd, filename);
		switch (type){
			case CMD_ls :
				ls_func(sockFD);
			break;

			case CMD_put :
				sprintf(path, "./client_folder_%d/", folder_index);
				strcat(path, filename);
				filesize = check_client_file_exist(sockFD, path);
				send_cmd_and_size(sockFD, writebuf, filesize);
				if (filesize >= 0){
					transmit_file(sockFD, path, filesize);
				}
				else
					fprintf(stderr, "The %s does not exist\n", filename);
			break;

			case CMD_get :
				sprintf(path, "./client_folder_%d/", folder_index);
				strcat(path, filename);
				filesize = check_server_file_exist(sockFD, writebuf);
				if (filesize >= 0){
					receive_file(sockFD, path, filesize);
				}
				else
					fprintf(stderr, "The %s does not exist\n", filename);
			break;

			case CMD_play :
				filesize = check_server_file_exist(sockFD, writebuf);
				if (filesize >= 0){
					if (ismpg(sockFD, &width, &height))
						streaming(sockFD, width, height, filesize);
					else
						fprintf(stderr, "The %s is not a mpg file\n", filename);
				}
				else
					fprintf(stderr, "The %s does not exist\n", filename);
			break;

			case CMD_not_found :
				fprintf(stderr, "Command not founded\n");
			break;

			case CMD_format_error :
				fprintf(stderr, "Command format error\n");
			break;

			case CMD_exit :
				fprintf(stderr, "Command is exit, now exit...\n");
				close(sockFD);
				exit(0);
			break;
		}
	}

	return 0;
}