#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "../include/proc.h"

#define BUF_SIZE 4096

static void read_file(const char *path) {
    int fd;
    ssize_t n;
    char buf[BUF_SIZE];

    fd = open(path, O_RDONLY);
    if (fd < 0) {
        perror(path);
        return;
    }

    while ((n = read(fd, buf, BUF_SIZE)) > 0) {
        write(STDOUT_FILENO, buf, n);
    }

    close(fd);
}

void read_cpuinfo(void) {
    printf("\n=== CPU INFO ===\n");
    read_file("/proc/cpuinfo");
}

void read_meminfo(void) {
    printf("\n=== MEMORY INFO ===\n");
    read_file("/proc/meminfo");
}

void list_processes(void) {
    DIR *dir;
    struct dirent *entry;
    char path[512];
    char line[256];

    dir = opendir("/proc");
    if (!dir) {
        perror("/proc");
        return;
    }

    printf("\n=== PROCESS LIST ===\n");
    printf("PID\tPPID\tSTATE\tNAME\n");

    while ((entry = readdir(dir)) != NULL) {
        if (!isdigit(entry->d_name[0]))
            continue;

        int written = snprintf(path, sizeof(path), "/proc/%s/status", entry->d_name);
        if (written < 0 || written >= (int)sizeof(path)) {
          continue;
        }
        FILE *fp = fopen(path, "r");
        if (!fp)
            continue;

        char name[64] = "";
        char state[8] = "";
        int ppid = 0;

        while (fgets(line, sizeof(line), fp)) {
            if (sscanf(line, "Name:\t%63s", name) == 1) {}
            else if (sscanf(line, "State:\t%7s", state) == 1) {}
            else if (sscanf(line, "PPid:\t%d", &ppid) == 1) {}
        }

        fclose(fp);
        printf("%s\t%d\t%s\t%s\n", entry->d_name, ppid, state, name);
    }

    closedir(dir);
}

