#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "parser.h"

#define MAX_IPS 1024
#define MAX_IP_LEN 64

typedef struct {
    char ip[MAX_IP_LEN];
    int count;
} ip_entry_t;

/* Known auth log locations (portable) */
static const char *auth_logs[] = {
    "/var/log/auth.log",   /* Debian/Ubuntu */
    "/var/log/secure",    /* RHEL/CentOS */
    NULL
};

/* Find a readable auth log */
static FILE *open_auth_log(void) {
    for (int i = 0; auth_logs[i]; i++) {
        FILE *fp = fopen(auth_logs[i], "r");
        if (fp)
            return fp;
    }
    return NULL;
}

/* Extract IPv4 address safely */
static int extract_ip(const char *line, char *out, size_t out_size) {
    const char *p = line;

    while (*p) {
        if (isdigit((unsigned char)*p)) {
            const char *start = p;
            while (*p && (isdigit((unsigned char)*p) || *p == '.'))
                p++;

            size_t len = (size_t)(p - start);
            if (len > 0 && len < out_size) {
                memcpy(out, start, len);
                out[len] = '\0';
                return 1;
            }
        }
        p++;
    }
    return 0;
}

void analyze_auth_log(void) {
    FILE *fp = open_auth_log();
    if (!fp) {
        fprintf(stderr, "No readable authentication log found.\n");
        return;
    }

    ip_entry_t ips[MAX_IPS];
    int ip_count = 0;

    char line[2048];

    while (fgets(line, sizeof(line), fp)) {
        if (!strstr(line, "Failed password"))
            continue;

        char ip[MAX_IP_LEN];

        if (!extract_ip(line, ip, sizeof(ip)))
            continue;

        int found = 0;
        for (int i = 0; i < ip_count; i++) {
            if (strcmp(ips[i].ip, ip) == 0) {
                ips[i].count++;
                found = 1;
                break;
            }
        }

        if (!found && ip_count < MAX_IPS) {
            strncpy(ips[ip_count].ip, ip, sizeof(ips[ip_count].ip) - 1);
            ips[ip_count].ip[sizeof(ips[ip_count].ip) - 1] = '\0';
            ips[ip_count].count = 1;
            ip_count++;
        }
    }

    fclose(fp);

    printf("\nFailed login attempts by IP:\n");
    printf("--------------------------------\n");

    for (int i = 0; i < ip_count; i++) {
        if (ips[i].count >= 3) {
            printf("[!] %s -> %d failures\n", ips[i].ip, ips[i].count);
        }
    }
}

