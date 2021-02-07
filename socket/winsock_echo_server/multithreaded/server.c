#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <process.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

static WSADATA wsaData;

typedef struct _remote_info_t
{
    int * done;
    int remote_sock;
    struct sockaddr_in remote_addr;
    socklen_t remote_addr_len;
} remote_info_t;

UINT WINAPI remote_socket_handler(void * arg)
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
    
    closesocket(info->remote_sock);

    free(info);
    
    return 0;
}

UINT WINAPI run_server(void * arg)
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
        HANDLE thread;
	    remote_info_t * info;
	    remote_sock = accept(sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
	    info = (remote_info_t*)malloc(sizeof(remote_info_t));
	    info->done = done;
	    info->remote_sock = remote_sock;
	    info->remote_addr = remote_addr;
	    info->remote_addr_len = remote_addr_len;

        _beginthreadex(NULL, 0, remote_socket_handler, info, 0, &thread);
	}
	
    }

    closesocket(sock);

    return 0;
}

int main(int argc, char *argv[])
{
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return -1;
    }

    HANDLE thread;
    int * done = (int*)malloc(sizeof(int));
    
    *done = 0;
    _beginthreadex(NULL, 0, run_server, done, 0, &thread);

    getchar();
    *done = 1;
    
    WaitForSingleObject(thread, INFINITE);

    free(done);

    WSACleanup();
    
    return 0;
}
