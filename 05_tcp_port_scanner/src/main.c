#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>

#include "scanner.h"

/* Attempt a TCP connect with timeout */
int scan_port(const char *ip, int port, int timeout_sec) {
    int sock;
    struct sockaddr_in addr;
    fd_set wfds;
    struct timeval tv;
    int flags, result;
    socklen_t len;
    int so_error;

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        return -1;

    flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &addr.sin_addr);

    result = connect(sock, (struct sockaddr *)&addr, sizeof(addr));
    if (result < 0 && errno != EINPROGRESS) {
        close(sock);
        return 0; /* immediate failure */
    }

    FD_ZERO(&wfds);
    FD_SET(sock, &wfds);

    tv.tv_sec = timeout_sec;
    tv.tv_usec = 0;

    result = select(sock + 1, NULL, &wfds, NULL, &tv);
    if (result <= 0) {
        close(sock);
        return 0; /* timeout or error */
    }

    len = sizeof(so_error);
    getsockopt(sock, SOL_SOCKET, SO_ERROR, &so_error, &len);

    close(sock);

    if (so_error == 0)
        return 1; /* open */

    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <ip> <start_port> <end_port>\n", argv[0]);
        return 1;
    }

    const char *ip = argv[1];
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);

    printf("Scanning %s from port %d to %d\n", ip, start, end);

    for (int port = start; port <= end; port++) {
        if (scan_port(ip, port, 1)) {
            printf("[OPEN ] TCP %d\n", port);
        }
    }

    return 0;
}

