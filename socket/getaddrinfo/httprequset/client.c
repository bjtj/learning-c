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
    char buffer[4096] = {0,};
    int len;
    struct addrinfo hint;
    struct addrinfo * res;
    memset(&hint, 0, sizeof(hint));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    assert(getaddrinfo("google.com", "80", &hint, &res) == 0);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    assert(connect(sock, res->ai_addr, res->ai_addrlen) == 0);

    freeaddrinfo(res);
    
    len = snprintf(buffer, sizeof(buffer), "GET / HTTP/1.1\r\n\r\n");
    assert(send(sock, buffer, len, 0) == len);
    memset(buffer, 0, sizeof(buffer));
    recv(sock, buffer, sizeof(buffer), 0);
    printf("%s\n", buffer);
    close(sock);
    
    return 0;
}
