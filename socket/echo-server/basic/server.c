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
	close(remote_sock);
    }

    close(sock);
    
    return 0;
}
