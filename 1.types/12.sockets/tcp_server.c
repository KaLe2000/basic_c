#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUF_SIZE 1024

int createServer(int port) {
    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server < 0) {
        perror("socket");
        return -1;
    }

    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };

    if (bind(server, (struct sockaddr*)&addr, sizeof(addr)) {
        perror("bind");
        close(server);
        return -1;
    }

    if (listen(server, 10)) {
        perror("listen");
        close(server);
        return -1;
    }

    return server;
}

void handleClient(int client) {
    char buf[BUF_SIZE];
    ssize_t n = recv(client, buf, sizeof(buf) - 1, 0);

    if (n > 0) {
        buf[n] = '\0';
        printf("Client: %s", buf);

        char response[BUF_SIZE];
        snprintf(response, sizeof(response), "Echo: %s", buf);
        send(client, response, strlen(response), 0);
    } else if (n == 0) {
        printf("Client disconnected\n");
    } else {
        perror("recv");
    }

    close(client);
}

int main() {
    int server = createServer(PORT);
    if (server < 0) return 1;

    printf("TCP Server listening on port %d\n", PORT);
    printf("Run: telnet localhost %d\n\n", PORT);

    while (1) {
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        int client = accept(server, (struct sockaddr*)&clientAddr, &addrLen);

        if (client < 0) {
            perror("accept");
            continue;
        }

        printf("New connection from %s:%d\n",
               inet_ntoa(clientAddr.sin_addr),
               ntohs(clientAddr.sin_port));

        handleClient(client);
    }

    close(server);
    return 0;
}