# Сокеты (Berkeley Sockets)

## Типы сокетов

| Тип | Протокол | Описание |
|-----|---------|---------|
| SOCK_STREAM | TCP | надёжный, ориентированный на соединение |
| SOCK_DGRAM | UDP | датаграммы, без соединения |
| SOCK_RAW | IP/ICMP | сырые сокеты |

## Адреса

### IPv4
```c
#include <netinet/in.h>

struct sockaddr_in {
    sa_family_t    sin_family;  // AF_INET
    in_port_t      sin_port;   // порт (network byte order)
    struct in_addr sin_addr;  // IP адрес
};

struct in_addr {
    uint32_t s_addr;  // network byte order
};
```

### IPv6
```c
struct sockaddr_in6 {
    sa_family_t     sin6_family;
    in_port_t       sin6_port;
    uint32_t        sin6_flowinfo;
    struct in6_addr sin6_addr;
    uint32_t        sin6_scope_id;
};
```

### Generic (для приведения типов)
```c
struct sockaddr {
    sa_family_t sa_family;
    char sa_data[14];
};
```

## Преобразование адресов

```c
#include <arpa/inet.h>

// IP → строка
inet_ntoa(addr.sin_addr);

// строка → IP (network order)
inet_pton(AF_INET, "192.168.1.1", &addr.sin_addr);

// localhost
addr.sin_addr.s_addr = htonl(INADDR_ANY);

// преобразования byte order
uint32_t htonl(uint32_t);  // host → network (long)
uint16_t htons(uint16_t);  // host → network (short)
uint32_t ntohl(uint32_t);  // network → host (long)
uint16_t ntohs(uint16_t);  // network → host (short)
```

## Создание сокета

```c
#include <sys/socket.h>

int sock = socket(AF_INET, SOCK_STREAM, 0);
if (sock < 0) {
    perror("socket");
}
```

## Привязка к порту

```c
struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(8080),  // порт
    .sin_addr.s_addr = htonl(INADDR_ANY)  // любой интерфейс
};

if (bind(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("bind");
}
```

## TCP server

```c
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

    int opt = 1;
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port),
        .sin_addr.s_addr = htonl(INADDR_ANY)
    };

    bind(server, (struct sockaddr*)&addr, sizeof(addr));
    listen(server, 10);  // backlog queue

    return server;
}

void handleClient(int client) {
    char buf[BUF_SIZE];
    ssize_t n = recv(client, buf, sizeof(buf) - 1, 0);
    if (n > 0) {
        buf[n] = '\0';
        printf("Received: %s\n", buf);
        send(client, buf, n, 0);  // echo
    }
    close(client);
}

int main() {
    int server = createServer(PORT);
    printf("Server listening on port %d\n", PORT);

    while (1) {
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        int client = accept(server, (struct sockaddr*)&clientAddr, &addrLen);

        if (client < 0) {
            perror("accept");
            continue;
        }

        handleClient(client);
    }

    close(server);
    return 0;
}
```

## TCP client

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

int connectToServer(const char* host, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };
    inet_pton(AF_INET, host, &addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("connect");
        close(sock);
        return -1;
    }

    return sock;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <host> <port>\n", argv[0]);
        return 1;
    }

    int sock = connectToServer(argv[1], atoi(argv[2]));
    if (sock < 0) return 1;

    char buf[1024];
    fgets(buf, sizeof(buf), stdin);
    send(sock, buf, strlen(buf), 0);

    ssize_t n = recv(sock, buf, sizeof(buf) - 1, 0);
    if (n > 0) {
        buf[n] = '\0';
        printf("Server: %s\n", buf);
    }

    close(sock);
    return 0;
}
```

## UDP

### UDP server
```c
int server = socket(AF_INET, SOCK_DGRAM, 0);
bind(server, ...);

struct sockaddr_in clientAddr;
char buf[1024];
socklen_t addrLen = sizeof(clientAddr);

ssize_t n = recvfrom(server, buf, sizeof(buf), 0,
                   (struct sockaddr*)&clientAddr, &addrLen);

sendto(server, buf, n, 0,
       (struct sockaddr*)&clientAddr, addrLen);
```

### UDP client
```c
int sock = socket(AF_INET, SOCK_DGRAM, 0);

struct sockaddr_in serverAddr = {
    .sin_family = AF_INET,
    .sin_port = htons(8080)
};
inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

sendto(sock, msg, strlen(msg), 0,
       (struct sockaddr*)&serverAddr, sizeof(serverAddr));
```

## Non-blocking I/O

```c
#include <fcntl.h>

int flags = fcntl(sock, F_GETFL, 0);
fcntl(sock, F_SETFL, flags | O_NONBLOCK);

// select / poll / epoll для multiplexing
```

## select

```c
#include <sys/select.h>

fd_set readfds;
FD_ZERO(&readfds);
FD_SET(server, &readfds);
FD_SET(client, &readfds);

int maxFd = (server > client) ? server : client;
struct timeval timeout = { .tv_sec = 5, .tv_usec = 0 };

int ready = select(maxFd + 1, &readfds, NULL, NULL, &timeout);

if (FD_ISSET(server, &readfds)) {
    // server готов
}
```

## getaddrinfo (network-agnostic)

```c
#include <netdb.h>

struct addrinfo hints = {
    .ai_family = AF_INET,    // или AF_INET6
    .ai_socktype = SOCK_STREAM
};

struct addrinfo *result;
int ret = getaddrinfo("example.com", "http", &hints, &result);

for (struct addrinfo* rp = result; rp != NULL; rp = rp->ai_next) {
    int sock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
    if (connect(sock, rp->ai_addr, rp->ai_addrlen) == 0) {
        // успешно
        break;
    }
    close(sock);
}

freeaddrinfo(result);
```

## Подводные камни

### 1. Byte order
```c
// Всегда используй htons/htonl для портов и адресов!
addr.sin_port = htons(8080);  // правильно
// addr.sin_port = 8080;       // неправильно на little-endian
```

### 2. sockaddr size
```c
socklen_t addrLen = sizeof(struct sockaddr_in);
// передавай по указателю в accept, recvfrom
```

### 3. Closed connections
```c
// recv возвращает 0 при закрытии connection
n = recv(sock, buf, size, 0);
if (n == 0) {
    // client закрыл connection
}
```

### 4. SIGPIPE
```c
// send на закрытый socket → SIGPIPE
// Решение: игнорировать или MSG_NOSIGNAL
send(sock, buf, len, MSG_NOSIGNAL);
```

### 5. Buffer sizes
```c
// TCP не гарантирует границы сообщений
// Всегда проверяй recv на полный буфер
ssize_t total = 0;
while (total < expected) {
    ssize_t n = recv(sock, buf + total, remaining, 0);
    if (n <= 0) break;
    total += n;
}
```

### 6. Socket reuse
```c
// SO_REUSEADDR для перезапуска сервера
int opt = 1;
setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```

## Полезные функции

```c
shutdown(sock, SHUT_RDWR);  // закрыть обе стороны
getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &len);
fcntl(sock, F_SETFD, FD_CLOEXEC);  // close on exec
```

## Компиляция

```bash
gcc -Wall -Wextra server.c -o server
gcc -Wall -Wextra client.c -o client
```