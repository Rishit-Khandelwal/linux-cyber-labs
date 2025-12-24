#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "honeypot.h"

#define BUFFER_SIZE 512
#define LOG_FILE "logs/ssh_honeypot.log"

static void safe_copy(char *dst, size_t dst_size, const char *src) {
    size_t len = strcspn(src, "\r\n");
    if (len >= dst_size)
        len = dst_size - 1;

    memcpy(dst, src, len);
    dst[len] = '\0';
}

static void log_attempt(const char *ip,
                        const char *user,
                        const char *pass) {
    FILE *fp = fopen(LOG_FILE, "a");
    if (!fp)
        return;

    time_t now = time(NULL);
    char *ts = ctime(&now);
    if (ts)
        ts[strlen(ts) - 1] = '\0';

    fprintf(fp, "[%s] IP=%s USER=%s PASS=%s\n",
            ts ? ts : "unknown", ip, user, pass);
    fclose(fp);
}

static void handle_client(int client_fd, const char *ip) {
    char buffer[BUFFER_SIZE];
    char username[64];
    char password[64];

    const char *banner = "SSH-2.0-OpenSSH_8.2p1 Ubuntu-4\r\n";
    send(client_fd, banner, strlen(banner), 0);

    send(client_fd, "login: ", 7, 0);
    ssize_t n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0)
        return;
    buffer[n] = '\0';
    safe_copy(username, sizeof(username), buffer);

    send(client_fd, "password: ", 10, 0);
    n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0)
        return;
    buffer[n] = '\0';
    safe_copy(password, sizeof(password), buffer);

    log_attempt(ip, username, password);

    send(client_fd, "Authentication failed.\n", 23, 0);
}

void run_honeypot(int port) {
    int sock;
    struct sockaddr_in addr;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    int opt = 1;
    setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }

    if (listen(sock, 5) < 0) {
        perror("listen");
        exit(1);
    }

    printf("SSH honeypot listening on port %d\n", port);

    while (1) {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);

        int client_fd = accept(sock, (struct sockaddr *)&client, &len);
        if (client_fd < 0)
            continue;

        char ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client.sin_addr, ip, sizeof(ip));

        handle_client(client_fd, ip);
        close(client_fd);
    }
}

int main(int argc, char *argv[]) {
    int port = 2222;
    if (argc == 2)
        port = atoi(argv[1]);

    run_honeypot(port);
    return 0;
}

