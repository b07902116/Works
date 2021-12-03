#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "opencv2/opencv.hpp"

#define DATASIZE 4072



using namespace cv;


typedef struct {
	int length;
	int seqNumber;
	int ackNumber;
	int fin;
	int syn;
	int ack;
} header;

typedef struct{
	header head;
	char data[DATASIZE];
} segment;



int main(int argc, char *argv[]){
	if (argc != 6){
		fprintf(stderr, "sender input format error\n");
		fprintf(stderr, "input should be ./sender <sender_ip> <agent_ip> <sender_port> <agent_port> <filepath>\n");
		fprintf(stderr, "For example ./sender 127.0.0.1 127.0.0.1 8887 8888 filepath\n");
		exit(1);
	}
	int sender_port, agent_port;
	char sender_ip[32] = {0}, agent_ip[32] = {0}, filepath[128] = {0};

	sscanf(argv[1], "%s", sender_ip);
	sscanf(argv[2], "%s", agent_ip);
	sscanf(argv[3], "%d", &sender_port);
	sscanf(argv[4], "%d", &agent_port);
	sscanf(argv[5], "%s", filepath);


	int senderFD;
	struct sockaddr_in sender_sock, agent_sock;

	// Create UDP socket
	senderFD = socket(PF_INET, SOCK_DGRAM, 0);

	// Set sender sockaddr_in struct
	sender_sock.sin_family = AF_INET;
	sender_sock.sin_port = htons(sender_port);
	sender_sock.sin_addr.s_addr = inet_addr(sender_ip);
	memset(sender_sock.sin_zero, '\0', sizeof(sender_sock.sin_zero));

	// bind
	bind(senderFD, (struct sockaddr *) &sender_sock, sizeof(sender_sock));

	// Set agent sockaddr_in struct
	agent_sock.sin_family = AF_INET;
	agent_sock.sin_port = htons(agent_port);
	agent_sock.sin_addr.s_addr = inet_addr(agent_ip);
	memset(agent_sock.sin_zero, '\0', sizeof(agent_sock.sin_zero));


	// Get video attribute
	int width, height, framenum, framesize;
	int framesize_pos = 0, frameid;
	int seq_num_interval, seq_num_next_frame = 1;
	VideoCapture cap;
	if (!cap.open(filepath)){
		fprintf(stderr, "the file is not a mpg file\n");
		exit(1);
	}
	width = cap.get(CV_CAP_PROP_FRAME_WIDTH);
	height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	framenum = cap.get(CV_CAP_PROP_FRAME_COUNT);
	framesize = width * height * 3;
	seq_num_interval = (framesize % DATASIZE == 0)? framesize / DATASIZE : framesize / DATASIZE + 1;

	// Store frame data
	Mat frame[2];
	frame[0] = Mat::zeros(height, width, CV_8UC3);
	frame[1] = Mat::zeros(height, width, CV_8UC3);
	uchar frame_buf[2][framesize];
	bzero(frame_buf[0], framesize);
	bzero(frame_buf[1], framesize);
	int inbuf_num[2] = {-1, -1};


	
	int seg_size = sizeof(segment);
	int send_size, recv_size, data_len;
	segment send_seg, recv_seg;

	int windowsize = 1, count = 0, send_num = 0, ack_num = -1, next_ack = -1;
	int threshold = 16;
	char buf[DATASIZE] = {0};


	struct timeval timeout;

	fd_set main_set, test_set;
	FD_ZERO(&main_set);
	FD_ZERO(&test_set);
	FD_SET(senderFD, &main_set);


	// send resolution to receiver
	while (ack_num < 0){
		sprintf(buf, "%d %d", height, width);
		bzero(&send_seg, seg_size);
		send_seg.head.length = strlen(buf);
		send_seg.head.seqNumber = 0;
		send_seg.head.fin = 0;
		strncpy(send_seg.data, buf, DATASIZE);
		send_size = sendto(senderFD, &send_seg, seg_size, 0, (struct sockaddr *) &agent_sock, sizeof(agent_sock));
		if (send_num == 0){
			printf("send    data    #0,	winSize = %d\n", windowsize);
			send_num++;
		}
		else{
			printf("resnd   data    #0,	winSize = %d\n", windowsize);
		}
		// receive ACK
		timeout.tv_sec = 0;
		timeout.tv_usec = 1000;
		memcpy(&test_set, &main_set, sizeof(main_set));
		if (select(senderFD + 1, &test_set, NULL, NULL, &timeout) > 0){
			recv_size = recvfrom(senderFD, &recv_seg, seg_size, 0, NULL, NULL);	// all msg come from agent
			printf("recv	ack     #%d\n", recv_seg.head.ackNumber);
			ack_num++;
		}
		else{
			threshold = (windowsize / 2 > 1)? windowsize / 2 : 1;
			windowsize = 1;
			printf("time    out,		threshold = %d\n", threshold);
		}
	}


	// Send data, ack_num starts from 0
	while (1){
		count = 1;
		next_ack = ack_num + windowsize;

		// send data
		while (count <= windowsize){
			// fill in segment
			bzero(&send_seg, seg_size);
			frameid = (ack_num + count - 1) / seq_num_interval;
			if (frameid > framenum){
				send_seg.head.length = 0;
				send_seg.head.seqNumber = ack_num + count;
				send_seg.head.fin = 1;
			}
			else{
				if (inbuf_num[frameid % 2] != frameid){
					cap.set(CV_CAP_PROP_POS_FRAMES, frameid);
					if (cap.read(frame[frameid % 2])){
						memcpy(frame_buf[frameid % 2], frame[frameid % 2].data, framesize);
					}
					inbuf_num[frameid % 2] = frameid;
				}
				framesize_pos = ((ack_num + count - 1) % seq_num_interval) * DATASIZE;
				data_len = (framesize - framesize_pos > DATASIZE)? DATASIZE : framesize - framesize_pos;
				memcpy(send_seg.data, &(frame_buf[frameid % 2][framesize_pos]), data_len);
				send_seg.head.length = data_len;
				send_seg.head.seqNumber = ack_num + count;
				send_seg.head.fin = 0;
			}


			// send packet to agent
			send_size = sendto(senderFD, &send_seg, seg_size, 0, (struct sockaddr *) &agent_sock, sizeof(agent_sock));
			if (ack_num + count == send_num){
				if (send_seg.head.fin)
					printf("send    fin\n");
				else
					printf("send    data    #%d,	winSize = %d\n",  ack_num + count, windowsize);
				send_num++;
			}
			else{
				if (send_seg.head.fin)
					printf("resnd   fin\n");
				else
					printf("resnd   data    #%d,	winSize = %d\n",  ack_num + count, windowsize);
			}

			// nonblocking receive ACK
			timeout.tv_sec = 0;
			timeout.tv_usec = 0;
			memcpy(&test_set, &main_set, sizeof(main_set));
			if (select(senderFD + 1, &test_set, NULL, NULL, &timeout) > 0){
				recv_size = recvfrom(senderFD, &recv_seg, seg_size, 0, NULL, NULL);	// all msg come from agent
				if (recv_seg.head.fin){
					printf("recv    finack\n");
					exit(0);
				}
				else{
					printf("recv	ack     #%d\n", recv_seg.head.ackNumber);
					if (recv_seg.head.ackNumber > ack_num){
						count -= (recv_seg.head.ackNumber - ack_num);
						ack_num = recv_seg.head.ackNumber;
						if (ack_num == next_ack){
							windowsize = (windowsize < threshold)? windowsize * 2 : windowsize + 1;
							next_ack = ack_num + windowsize;
						}
					}
				}
			}
			count++;
			if (send_seg.head.fin)
				break;
		}

		// sender set timer and wait ACK
		while (1){
			timeout.tv_sec = 0;
			timeout.tv_usec = 500000;
			memcpy(&test_set, &main_set, sizeof(main_set));
			if (select(senderFD + 1, &test_set, NULL, NULL, &timeout) > 0){
				// receive ACK
				recv_size = recvfrom(senderFD, &recv_seg, seg_size, 0, NULL, NULL);	// all msg come from agent
				if (recv_seg.head.fin){
					printf("recv    finack\n");
					exit(0);
				}
				else{
					printf("recv	ack     #%d\n", recv_seg.head.ackNumber);
					if (recv_seg.head.ackNumber > ack_num){
						ack_num = recv_seg.head.ackNumber;
						if (ack_num == next_ack){
							windowsize = (windowsize < threshold)? windowsize * 2 : windowsize + 1;
							next_ack = ack_num + windowsize;
							break;
						}
					}
				}
			}
			else{
				printf("time    out,		threshold = %d\n", threshold);
				threshold = (windowsize / 2 > 1)? windowsize / 2 : 1;
				windowsize = 1;
				break;
			}
		}
	}
	cap.release();
	return 0;
}