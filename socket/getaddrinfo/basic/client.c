#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char *argv[])
{
    int sock;
    char buffer[100] = {0,};
    int len;
    struct addrinfo hint;
    struct addrinfo * res;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    assert(getaddrinfo("127.0.0.1", "8090", &hint, &res) == 0);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(connect(sock, res->ai_addr, res->ai_addrlen) == 0);

    freeaddrinfo(res);
    
    len = snprintf(buffer, sizeof(buffer), "Hello!");
    assert(send(sock, buffer, len, 0) == len);
    memset(buffer, 0, sizeof(buffer));
    assert(recv(sock, buffer, sizeof(buffer), 0) == len);
    assert(strcmp(buffer, "Hello!") == 0);
    close(sock);
    
    return 0;
}
