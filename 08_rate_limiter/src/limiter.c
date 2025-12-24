#include <string.h>
#include <time.h>
#include <stdio.h>

#include "limiter.h"

static client_t clients[MAX_CLIENTS];
static int client_count = 0;

static client_t *find_client(const char *ip) {
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].ip, ip) == 0)
            return &clients[i];
    }
    return NULL;
}

int allow_request(const char *ip) {
    time_t now = time(NULL);
    client_t *c = find_client(ip);

    if (!c) {
        if (client_count >= MAX_CLIENTS)
            return 0;

        c = &clients[client_count++];
        strncpy(c->ip, ip, sizeof(c->ip) - 1);
        c->ip[sizeof(c->ip) - 1] = '\0';
        c->count = 1;
        c->window_start = now;
        return 1;
    }

    if (now - c->window_start > WINDOW_SEC) {
        c->count = 1;
        c->window_start = now;
        return 1;
    }

    if (c->count >= MAX_REQ)
        return 0;

    c->count++;
    return 1;
}

