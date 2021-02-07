#define _WINSOCK_DEPRECATED_NO_WARNINGS

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
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);
    char buffer[100] = {0,};
    int len;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(8090);
    connect(sock, (struct sockaddr*)&addr, addr_len);
    len = snprintf(buffer, sizeof(buffer), "Hello!");
    assert(send(sock, buffer, len, 0) == len);
    memset(buffer, 0, sizeof(buffer));
    assert(recv(sock, buffer, sizeof(buffer), 0) == len);
    assert(strcmp(buffer, "Hello!") == 0);
    closesocket(sock);

    WSACleanup();
    
    return 0;
}
