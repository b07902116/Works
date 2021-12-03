#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define ERR_EXIT(a) { perror(a); exit(1); }

typedef struct {
    char hostname[512];  // server's hostname
    unsigned short port;  // port to listen
    int listen_fd;  // fd to wait for a new connection
} server;

typedef struct {
    char host[512];  // client's host
    int conn_fd;  // fd to talk with client
    char buf[512];  // data sent by/to client
    size_t buf_len;  // bytes used by buf
    // you don't need to change this.
	int item;
    int wait_for_write;  // used by handle_read to know if the header is read or not.
} request;


typedef struct {
    int id;
    int balance;
} Account;


server svr;  // server
request* requestP = NULL;  // point to a list of requests
int maxfd;  // size of open file descriptor table, size of request list

const char* accept_read_header = "ACCEPT_FROM_READ";
const char* accept_write_header = "ACCEPT_FROM_WRITE";

// Forwards

static void init_server(unsigned short port);
// initailize a server, exit for error

static void init_request(request* reqP);
// initailize a request instance

static void free_request(request* reqP);
// free resources used by a request instance

static int handle_read(request* reqP);
// return 0: socket ended, request done.
// return 1: success, message (without header) got this time is in reqP->buf with reqP->buf_len bytes. read more until got <= 0.
// It's guaranteed that the header would be correctly set after the first read.
// error code:
// -1: client connection error

int main(int argc, char** argv) {


    int i, ret;

    struct sockaddr_in cliaddr;  // used by accept()
    int clilen;

    int conn_fd;  // fd for a new connection with client
    int file_fd;  // fd for file that we open for reading
    char buf[512];
    int buf_len;

    // Parse args.
    if (argc != 2) {
        fprintf(stderr, "usage: %s [port]\n", argv[0]);
        exit(1);
    }

    // Initialize server
    init_server((unsigned short) atoi(argv[1]));

    // Get file descripter table size and initize request table
    maxfd = getdtablesize();
    requestP = (request*) malloc(sizeof(request) * maxfd);
    if (requestP == NULL) {
        ERR_EXIT("out of memory allocating all requests");
    }
    for (i = 0; i < maxfd; i++) {
        init_request(&requestP[i]);
    }
    requestP[svr.listen_fd].conn_fd = svr.listen_fd;
    strcpy(requestP[svr.listen_fd].host, svr.hostname);

    // Loop for handling connections
    fprintf(stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d...\n", svr.hostname, svr.port, svr.listen_fd, maxfd);


    const char *fail = "Operation failed.\n";
    const char *success = "This account is modifiable.\n";
    const char *locked = "This account is locked.\n";
    int id, result, number, number2, nbuf[2], record_lock[21] = {0};
    char cmd[512];

    struct flock lock_data;

    struct usr{
        int step, id, pos;
    };
    struct usr info[maxfd];
    for (i = 0; i < maxfd; i++)
        info[i].step = 0;

    struct timeval timeout;
    fd_set  master_set, working_set;
    FD_ZERO(&master_set);
    timeout.tv_sec = 0;
    timeout.tv_usec = 5000;
    FD_SET(svr.listen_fd, &master_set);

    while (1) {
        // TODO: Add IO multiplexing
        memcpy(&working_set, &master_set, sizeof(master_set));
        if (select(maxfd, &working_set, NULL, NULL, &timeout) > 0){
            if (FD_ISSET(svr.listen_fd, &working_set)){
                // Check new connection--------------------------------------
                clilen = sizeof(cliaddr);
                conn_fd = accept(svr.listen_fd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen);
                if (conn_fd < 0) {
                    if (errno == EINTR || errno == EAGAIN) continue;  // try again
                    if (errno == ENFILE) {
                        (void) fprintf(stderr, "out of file descriptor table ... (maxconn %d)\n", maxfd);
                        continue;
                    }
                    ERR_EXIT("accept")
                }
                requestP[conn_fd].conn_fd = conn_fd;
                strcpy(requestP[conn_fd].host, inet_ntoa(cliaddr.sin_addr));
                fprintf(stderr, "getting a new request... fd %d from %s\n", conn_fd, requestP[conn_fd].host);

                FD_SET(conn_fd, &master_set);
            }
        }


        memcpy(&working_set, &master_set, sizeof(master_set));
        if (select(maxfd, &working_set, NULL, NULL, &timeout) > 0){
            for (i = 4; i < maxfd; i++){
                if (FD_ISSET(i, &working_set)){
                    conn_fd = i;
                    break;
                }
            }
            ret = handle_read(&requestP[conn_fd]); // parse data from client to requestP[conn_fd].buf
            if (ret < 0) {
                fprintf(stderr, "bad request from %s\n", requestP[conn_fd].host);
                continue;
            }
            info[conn_fd].step++;
        }
#ifdef READ_SERVER
        for (i = 4; i < maxfd; i++){
            if (info[i].step == 1){
                conn_fd = i;
                sscanf(requestP[conn_fd].buf, "%d", &id);
                file_fd = open("account_list", O_RDONLY);
                lseek(file_fd, 2 * sizeof(int) * (id - 1), SEEK_SET);

                lock_data.l_type = F_RDLCK;
                lock_data.l_whence = SEEK_CUR;
                lock_data.l_start = 0;
                lock_data.l_len = 2 * sizeof(int);
                fcntl(file_fd, F_GETLK, &lock_data);
                if (lock_data.l_type == F_UNLCK){
                    read(file_fd, &nbuf[0], sizeof(int));
                    read(file_fd, &nbuf[1], sizeof(int));
                    sprintf(buf, "%d %d", nbuf[0], nbuf[1]);
                    write(requestP[conn_fd].conn_fd, buf, strlen(buf));
                    write(requestP[conn_fd].conn_fd, "\n", 1);
                }
                else
                    write(requestP[conn_fd].conn_fd, locked, strlen(locked));

                close(requestP[conn_fd].conn_fd);
                free_request(&requestP[conn_fd]);
                FD_CLR(conn_fd, &master_set);
                info[conn_fd].step = 0;
            }
        }
#else
        for (i = 4; i < maxfd; i++){
            if (info[i].step == 1){
                conn_fd = i;
                sscanf(requestP[conn_fd].buf, "%d", &info[conn_fd].id);

                file_fd = open("account_list", O_RDWR);
                info[conn_fd].pos = 2 * sizeof(int) * (info[conn_fd].id - 1);
                lock_data.l_type = F_WRLCK;
                lock_data.l_whence = SEEK_SET;
                lock_data.l_start = info[conn_fd].pos;
                lock_data.l_len = 2 * sizeof(int);
                fcntl(file_fd, F_GETLK, &lock_data);
                if ((record_lock[info[conn_fd].id] != 0) || (lock_data.l_type != F_UNLCK)){
                    write(requestP[conn_fd].conn_fd, locked, strlen(locked));
                    close(requestP[conn_fd].conn_fd);
                    free_request(&requestP[conn_fd]);
                    FD_CLR(conn_fd, &master_set);
                    info[i].step = 0;
                }
                lock_data.l_type = F_WRLCK;
                lock_data.l_whence = SEEK_SET;
                lock_data.l_start = info[conn_fd].pos;
                lock_data.l_len = 2 * sizeof(int);

                if (fcntl(file_fd, F_SETLK, &lock_data) != -1){
                    write(requestP[conn_fd].conn_fd, success, strlen(success));
                    record_lock[info[conn_fd].id] = 1;
                    info[conn_fd].step = 2;
                }
                else{
                    write(requestP[conn_fd].conn_fd, locked, strlen(locked));
                    close(requestP[conn_fd].conn_fd);
                    free_request(&requestP[conn_fd]);
                    FD_CLR(conn_fd, &master_set);
                    info[i].step = 0;
                }
            }
            else if (info[i].step == 3){
                conn_fd = i;
                sscanf(requestP[conn_fd].buf, "%s %d", cmd, &number);
                lseek(file_fd, info[conn_fd].pos, SEEK_SET);
                read(file_fd, nbuf, 2 * sizeof(int));
                if (strcmp(cmd, "save") == 0){
                    if (number >= 0)
                        nbuf[1] += number;
                    else
                        write(requestP[conn_fd].conn_fd, fail, strlen(fail));
                }
                else if (strcmp(cmd, "withdraw") == 0){
                    if (number <= nbuf[1] && number >= 0)
                        nbuf[1] -= number;
                    else
                        write(requestP[conn_fd].conn_fd, fail, strlen(fail));
                }
                else if (strcmp(cmd, "balance") == 0){
                    if (number > 0)
                        nbuf[1] = number;
                    else
                        write(requestP[conn_fd].conn_fd, fail, strlen(fail));
                }
                else if (strcmp(cmd, "transfer") == 0){
                    sscanf(requestP[conn_fd].buf, "%s%d%d", cmd, &number, &number2);//number is account, number2 is money
                    if (number2 <= nbuf[1] && number2 >= 0)
                        nbuf[1] -= number2;
                    else
                        write(requestP[conn_fd].conn_fd, fail, strlen(fail));
                }
                else
                    write(requestP[conn_fd].conn_fd, fail, strlen(fail));

                lseek(file_fd, info[conn_fd].pos, SEEK_SET);
                write(file_fd, nbuf, 2 * sizeof(int));
                if (strcmp(cmd, "transfer") == 0){
                    lseek(file_fd, 2 * sizeof(int) * (number - 1), SEEK_SET);
                    read(file_fd, nbuf, 2 * sizeof(int));
                    nbuf[1] += number2;
                    lseek(file_fd, 2 * sizeof(int) * (number - 1), SEEK_SET);
                    write(file_fd, nbuf, 2 * sizeof(int));
                }
                close(requestP[conn_fd].conn_fd);
                free_request(&requestP[conn_fd]);
                FD_CLR(conn_fd, &master_set);
                lock_data.l_type = F_UNLCK;
                fcntl(file_fd, F_SETLK, &lock_data);
                record_lock[info[conn_fd].id] = 0;
                info[conn_fd].step = 0;
            }
        }
#endif
    }
    free(requestP);
    return 0;
}


// ======================================================================================================
// You don't need to know how the following codes are working
#include <fcntl.h>

static void* e_malloc(size_t size);


static void init_request(request* reqP) {
    reqP->conn_fd = -1;
    reqP->buf_len = 0;
    reqP->item = 0;
    reqP->wait_for_write = 0;
}

static void free_request(request* reqP) {
    /*if (reqP->filename != NULL) {
        free(reqP->filename);
        reqP->filename = NULL;
    }*/
    init_request(reqP);
}

// return 0: socket ended, request done.
// return 1: success, message (without header) got this time is in reqP->buf with reqP->buf_len bytes. read more until got <= 0.
// It's guaranteed that the header would be correctly set after the first read.
// error code:
// -1: client connection error
static int handle_read(request* reqP) {
    int r;
    char buf[512];

    // Read in request from client
    r = read(reqP->conn_fd, buf, sizeof(buf));
    if (r < 0) return -1;
    if (r == 0) return 0;
	char* p1 = strstr(buf, "\015\012");
	int newline_len = 2;
	// be careful that in Windows, line ends with \015\012
	if (p1 == NULL) {
		p1 = strstr(buf, "\012");
		newline_len = 1;
		if (p1 == NULL) {
			ERR_EXIT("this really should not happen...");
		}
	}
	size_t len = p1 - buf + 1;
	memmove(reqP->buf, buf, len);
	reqP->buf[len - 1] = '\0';
	reqP->buf_len = len-1;
    return 1;
}

static void init_server(unsigned short port) {
    struct sockaddr_in servaddr;
    int tmp;

    gethostname(svr.hostname, sizeof(svr.hostname));
    svr.port = port;

    svr.listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (svr.listen_fd < 0) ERR_EXIT("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    tmp = 1;
    if (setsockopt(svr.listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) {
        ERR_EXIT("setsockopt");
    }
    if (bind(svr.listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        ERR_EXIT("bind");
    }
    if (listen(svr.listen_fd, 1024) < 0) {
        ERR_EXIT("listen");
    }
}

static void* e_malloc(size_t size) {
    void* ptr;

    ptr = malloc(size);
    if (ptr == NULL) ERR_EXIT("out of memory");
    return ptr;
}

