#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "limiter.h"

#define PORT 8080
#define LOG_FILE "logs/rate_limiter.log"

static void log_event(const char *ip, const char *action) {
    FILE *fp = fopen(LOG_FILE, "a");
    if (!fp)
        return;

    time_t now = time(NULL);
    fprintf(fp, "[%ld] IP=%s ACTION=%s\n", now, ip, action);
    fclose(fp);
}

int main(void) {
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(sock, 10) < 0) {
        perror("listen");
        return 1;
    }

    printf("Rate limiter listening on port %d\n", PORT);

    while (1) {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);
        int cfd = accept(sock, (struct sockaddr *)&client, &len);
        if (cfd < 0)
            continue;

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.sin_addr, ip, sizeof(ip));

        if (!allow_request(ip)) {
            log_event(ip, "BLOCKED");
            close(cfd);
            continue;
        }

        log_event(ip, "ALLOWED");
        send(cfd, "OK\n", 3, 0);
        close(cfd);
    }

    return 0;
}

