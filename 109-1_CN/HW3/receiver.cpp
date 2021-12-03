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
	if (argc != 5){
		fprintf(stderr, "receiver input format error\n");
		fprintf(stderr, "input should be ./receiver <receiver_ip> <agent_ip> <receiver_port> <agent_port>\n");
		fprintf(stderr, "For example ./receiver 127.0.0.1 127.0.0.1 8889 8888\n");
		exit(1);
	}
	int receiver_port, agent_port;
	char receiver_ip[32] = {0}, agent_ip[32] = {0};

	sscanf(argv[1], "%s", receiver_ip);
	sscanf(argv[2], "%s", agent_ip);
	sscanf(argv[3], "%d", &receiver_port);
	sscanf(argv[4], "%d", &agent_port);


	int receiverFD;
	struct sockaddr_in receiver_sock, agent_sock;

	// Create UDP socket
	receiverFD = socket(PF_INET, SOCK_DGRAM, 0);

	// Set receiver sockaddr_in struct
	receiver_sock.sin_family = AF_INET;
	receiver_sock.sin_port = htons(receiver_port);
	receiver_sock.sin_addr.s_addr = inet_addr(receiver_ip);
	memset(receiver_sock.sin_zero, '\0', sizeof(receiver_sock.sin_zero));

	// bind
	bind(receiverFD, (struct sockaddr *) &receiver_sock, sizeof(receiver_sock));

	// Set agent sockaddr_in struct
	agent_sock.sin_family = AF_INET;
	agent_sock.sin_port = htons(agent_port);
	agent_sock.sin_addr.s_addr = inet_addr(agent_ip);
	memset(agent_sock.sin_zero, '\0', sizeof(agent_sock.sin_zero));
	

	
	int seg_size = sizeof(segment);
	int send_size, recv_size;
	segment send_seg, recv_seg;

	int next_num = 1, finish = 0;

	int width, height, framesize;
	int framesize_pos = 0, frameid = 0;
	int seq_num_interval, seq_num_next_frame, now_frame_len;
	int flush_size = 0;
	int playid = -1;
	char c;
	char recv_buf[32 * seg_size];
	bzero(recv_buf, sizeof(recv_buf));





	// receiver resolution
	recv_size = recvfrom(receiverFD, &recv_seg, seg_size, 0, NULL, NULL);	// all msg come from agent
	printf("recv    data    #0\n");
	sscanf(recv_seg.data, "%d%d", &height, &width);
	send_seg.head.ackNumber = 0;
	send_seg.head.ack = 1;
	send_size = sendto(receiverFD, &send_seg, seg_size, 0, (struct sockaddr *) &agent_sock, sizeof(agent_sock));
	printf("send    ack     #0\n");

	framesize = width * height * 3;
	seq_num_interval = (framesize % DATASIZE == 0)? framesize / DATASIZE : framesize / DATASIZE + 1;
	seq_num_next_frame = 1 + seq_num_interval;


	// Store frame data
	Mat frame;
	frame = Mat::zeros(height, width, CV_8UC3);
	uchar frame_buf[2][framesize];
	bzero(frame_buf[0], framesize);
	bzero(frame_buf[1], framesize);



	while (!finish){
		// receive segment
		bzero(&recv_seg, seg_size);
		recv_size = recvfrom(receiverFD, &recv_seg, seg_size, 0, NULL, NULL);	// all msg come from agent
		if (recv_size > 0){
			if (recv_seg.head.seqNumber == next_num){

				// if the received packet cause overflow, drop it 
				// Process received data
				if ((flush_size + recv_seg.head.length > sizeof(recv_buf)) || recv_seg.head.fin){
					if (recv_seg.head.fin)
						printf("recv    fin\n");
					else
						printf("drop    data    #%d\n", recv_seg.head.seqNumber);
					// the frameid of the last packet in recv_buf
					frameid = (next_num - 1 - 1) / seq_num_interval;
					if (framesize_pos + flush_size > framesize){
						// recv_buf contains data crossing two frames
						memcpy(&(frame_buf[(frameid + 1) % 2][framesize_pos]), recv_buf, now_frame_len);
						memcpy(&(frame_buf[frameid % 2]), &(recv_buf[now_frame_len]), flush_size - now_frame_len);
						framesize_pos = flush_size - now_frame_len;
						playid = (frameid + 1) % 2;
					}
					else if (framesize_pos + flush_size == framesize){
						// recv_buf contains exactly the last part of a frame
						memcpy(&(frame_buf[frameid % 2][framesize_pos]), recv_buf, flush_size);
						framesize_pos += flush_size;
						playid = frameid % 2;
					}
					else{
						// recv_buf contains the middle part of a frame
						memcpy(&(frame_buf[frameid % 2][framesize_pos]), recv_buf, flush_size);
						framesize_pos += flush_size;
						playid = -1;
					}
					flush_size = 0;

					bzero(&send_seg, seg_size);
					send_seg.head.ackNumber = next_num - 1;
					send_seg.head.ack = 1;
					send_seg.head.fin = recv_seg.head.fin? 1 : 0;
					// send ACK
					send_size = sendto(receiverFD, &send_seg, seg_size, 0, (struct sockaddr *) &agent_sock, sizeof(agent_sock));
					if (send_seg.head.fin){
						printf("send    finack\n");
						finish = 1;
					}
					else
						printf("send    ack     #%d\n", send_seg.head.ackNumber);
					printf("flush\n");
				}
				else{
					printf("recv    data    #%d\n", next_num);
					if (next_num == seq_num_next_frame){
						now_frame_len = flush_size;
						seq_num_next_frame += seq_num_interval;
					}
					memcpy(&(recv_buf[flush_size]), recv_seg.data, recv_seg.head.length);
					flush_size += recv_seg.head.length;

					bzero(&send_seg, seg_size);
					send_seg.head.ackNumber = next_num;
					send_seg.head.ack = 1;
					send_seg.head.fin = 0;
					// send ACK
					send_size = sendto(receiverFD, &send_seg, seg_size, 0, (struct sockaddr *) &agent_sock, sizeof(agent_sock));
					if (send_seg.head.fin)
						printf("send    finack\n");
					else
						printf("send    ack     #%d\n", send_seg.head.ackNumber);

					next_num++;
				}

			}
			else{
				// this segment is out of order, discard
				bzero(&send_seg, seg_size);
				send_seg.head.ackNumber = next_num - 1;
				send_seg.head.ack = 1;
				send_seg.head.fin = 0;
				printf("drop    data    #%d\n", recv_seg.head.seqNumber);
				send_size = sendto(receiverFD, &send_seg, seg_size, 0, (struct sockaddr *) &agent_sock, sizeof(agent_sock));
				printf("send    ack     #%d\n", next_num - 1);
			}
		}
		else{
			perror("recvfrom : ");
			exit(0);
		}
		
		// play frame
		if (playid >= 0){
			memcpy(frame.data, frame_buf[playid], framesize);
			imshow("Video", frame);
			c = (char) waitKey(33.3333);
			if (c == 27){
				bzero(&send_seg, seg_size);
				send_seg.head.ack = 1;
				send_seg.head.fin = 1;
				send_size = sendto(receiverFD, &send_seg, seg_size, 0, (struct sockaddr *) &agent_sock, sizeof(agent_sock));
				finish = 1;
				printf("send    finack\n");
				break;
			}
		}
	}
	destroyAllWindows();
	return 0;
}