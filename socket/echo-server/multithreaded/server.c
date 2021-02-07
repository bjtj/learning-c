#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <pthread.h>

typedef struct _remote_info_t
{
    int * done;
    int remote_sock;
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len;
} remote_info_t;

void * remote_socket_handler(void * arg)
{
    remote_info_t * info = (remote_info_t*)arg;
    char buffer[100] = {0,};
    int len;

    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    struct timeval timeout;
    int maxfd;

    int received;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    maxfd = info->remote_sock;
    FD_SET(info->remote_sock, &readfds);
    FD_SET(info->remote_sock, &writefds);

    received = 0;

    while (!*(info->done)) {

	int ret;
	fd_set curr_readfds = readfds;
	fd_set curr_writefds = writefds;
	fd_set curr_exceptfds = exceptfds;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;

	if ((ret = select(maxfd + 1, &curr_readfds, &curr_writefds, &curr_exceptfds, &timeout)) == 0) {
	    continue;
	}

	if (ret < 0) {
	    break;
	}

	if (FD_ISSET(info->remote_sock, &curr_readfds) && received == 0) {
	    len = recv(info->remote_sock, buffer, sizeof(buffer), 0);
	    if (len == 0) {
		break;
	    }
	    received = 1;
	}

	if (FD_ISSET(info->remote_sock, &curr_writefds) && received == 1) {
	    send(info->remote_sock, buffer, len, 0);
	    break;
	}
    }
    
    close(info->remote_sock);

    free(info);
    
    return 0;
}

void * run_server(void * arg)
{
    int * done = (int*)arg;
    int sock;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int on = 1;

    fd_set readfds;
    fd_set writefds;
    fd_set exceptfds;
    struct timeval timeout;
    int maxfd;

    FD_ZERO(&readfds);
    FD_ZERO(&writefds);
    FD_ZERO(&exceptfds);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    maxfd = sock;
    FD_SET(sock, &readfds);

    assert(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8090);
    
    assert(bind(sock, (struct sockaddr*)&addr, addr_len) == 0);
    assert(listen(sock, 5) == 0);

    while (!*done) {
	int remote_sock;
	struct sockaddr_in remote_addr;
	socklen_t remote_addr_len = sizeof(remote_addr);
	int ret;
	fd_set curr_readfds = readfds;
	fd_set curr_writefds = writefds;
	fd_set curr_exceptfds = exceptfds;
	timeout.tv_sec = 1;
	timeout.tv_usec = 0;
    
	if ((ret = select(maxfd + 1, &curr_readfds, &curr_writefds, &curr_exceptfds, &timeout)) == 0) {
	    continue;
	}

	if (ret < 0) {
	    break;
	}

	if (FD_ISSET(sock, &readfds)) {
	    pthread_attr_t attr;
	    pthread_t pid;
	    remote_info_t * info;
	    remote_sock = accept(sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
	    info = (remote_info_t*)malloc(sizeof(remote_info_t));
	    info->done = done;
	    info->remote_sock = remote_sock;
	    info->remote_addr = remote_addr;
	    info->remote_addr_len = remote_addr_len;

	    assert(pthread_attr_init(&attr) == 0);
	    assert(pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED) == 0);
	    pthread_create(&pid, &attr, remote_socket_handler, info);
	    assert(pthread_attr_destroy(&attr) == 0);
	}
	
    }

    close(sock);

    return 0;
}

int main(int argc, char *argv[])
{
    int * done = (int*)malloc(sizeof(int));
    pthread_t pid;
    *done = 0;
    pthread_create(&pid, NULL, run_server, done);

    getchar();
    *done = 1;
    
    pthread_join(pid, NULL);

    free(done);
    
    return 0;
}
