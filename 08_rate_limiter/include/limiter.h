#ifndef LIMITER_H
#define LIMITER_H

#include <time.h>

#define MAX_CLIENTS 1024
#define MAX_REQ     5      /* max requests */
#define WINDOW_SEC  10     /* per 10 seconds */

typedef struct {
    char ip[64];
    int count;
    time_t window_start;
} client_t;

int allow_request(const char *ip);

#endif

