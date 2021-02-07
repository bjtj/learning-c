#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <ws2ipdef.h>
#include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "iphlpapi.lib")

static WSADATA wsaData;

int main(int argc, char *argv[])
{

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return -1;
    }

    int sock;
    int remote_sock;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    int on = 1;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(sock >= 0);

    assert(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(8090);
    
    assert(bind(sock, (struct sockaddr*)&addr, addr_len) == 0);
    assert(listen(sock, 5) == 0);

    while (1) {
	struct sockaddr_in remote_addr;
	socklen_t remote_addr_len = sizeof(remote_addr);
	char buffer[100] = {0,};
	int len;
    
	remote_sock = accept(sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
	len = recv(remote_sock, buffer, sizeof(buffer), 0);
	send(remote_sock, buffer, len, 0);
	closesocket(remote_sock);
    }

    closesocket(sock);

    WSACleanup();
    
    return 0;
}
