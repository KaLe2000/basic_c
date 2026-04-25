#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        fprintf(stderr, "Example: %s localhost 8080\n", argv[0]);
        return 1;
    }

    const char* host = argv[1];
    int port = atoi(argv[2]);

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };

    if (inet_pton(AF_INET, host, &addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return 1;
    }

    printf("Connecting to %s:%d...\n", host, port);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) {
        perror("connect");
        close(sock);
        return 1;
    }

    printf("Connected! Type message (Ctrl+C to exit):\n\n");

    char buf[1024];
    while (fgets(buf, sizeof(buf), stdin)) {
        if (send(sock, buf, strlen(buf), 0) < 0) {
            perror("send");
            break;
        }

        memset(buf, 0, sizeof(buf));
        ssize_t n = recv(sock, buf, sizeof(buf) - 1, 0);

        if (n > 0) {
            buf[n] = '\0';
            printf("Server: %s", buf);
        } else if (n == 0) {
            printf("Server closed connection\n");
            break;
        } else {
            perror("recv");
            break;
        }
    }

    close(sock);
    return 0;
}