#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
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
    close(sock);
    
    return 0;
}
