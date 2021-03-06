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

UINT WINAPI run_server(void * arg)
{
    int * done = (int*)arg;
    int sock;
    int remote_sock;
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
	struct sockaddr_in remote_addr;
	socklen_t remote_addr_len = sizeof(remote_addr);
	char buffer[100] = {0,};
	int len;
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
	    remote_sock = accept(sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
	    len = recv(remote_sock, buffer, sizeof(buffer), 0);
	    send(remote_sock, buffer, len, 0);
	    closesocket(remote_sock);
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
