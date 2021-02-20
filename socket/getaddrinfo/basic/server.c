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
    int remote_sock;
    int on = 1;

    struct addrinfo hint;
    struct addrinfo * res;
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_INET;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
    assert(getaddrinfo("127.0.0.1", "8090", &hint, &res) == 0);

    sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    assert(sock >= 0);

    assert(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == 0);

    assert(bind(sock, res->ai_addr, res->ai_addrlen) == 0);
    assert(listen(sock, 5) == 0);

    freeaddrinfo(res);

    while (1) {
	struct sockaddr_in remote_addr;
	socklen_t remote_addr_len = sizeof(remote_addr);
	char buffer[100] = {0,};
	int len;
    
	remote_sock = accept(sock, (struct sockaddr*)&remote_addr, &remote_addr_len);
	len = recv(remote_sock, buffer, sizeof(buffer), 0);
	send(remote_sock, buffer, len, 0);
	close(remote_sock);
    }

    close(sock);
    
    return 0;
}
