#include <dirent.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "monitor.h"

/* Read process name safely from /proc/[pid]/status */
static int read_process_name(const char *pid, char *name, size_t size) {
    char path[256];
    char line[256];

    int written = snprintf(path, sizeof(path), "/proc/%s/status", pid);
    if (written < 0 || written >= (int)sizeof(path))
        return 0;

    FILE *fp = fopen(path, "r");
    if (!fp)
        return 0;

    while (fgets(line, sizeof(line), fp)) {
        if (sscanf(line, "Name:%63s", name) == 1) {
            name[size - 1] = '\0';   /* hard safety */
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void take_snapshot(snapshot_t *snap) {
    DIR *dir;
    struct dirent *entry;

    snap->count = 0;
    dir = opendir("/proc");
    if (!dir)
        return;

    while ((entry = readdir(dir)) != NULL) {
        if (!isdigit(entry->d_name[0]))
            continue;

        if (snap->count >= MAX_PROCS)
            break;

        process_t *p = &snap->procs[snap->count];

        p->pid = atoi(entry->d_name);
        p->first_seen = time(NULL);

        if (!read_process_name(entry->d_name, p->name, sizeof(p->name))) {
            strncpy(p->name, "?", sizeof(p->name));
            p->name[sizeof(p->name) - 1] = '\0';
        }

        snap->count++;
    }

    closedir(dir);
}

static process_t *find_process(snapshot_t *snap, int pid) {
    for (int i = 0; i < snap->count; i++) {
        if (snap->procs[i].pid == pid)
            return &snap->procs[i];
    }
    return NULL;
}

void compare_snapshots(snapshot_t *old, snapshot_t *new) {
    /* New processes or PID reuse */
    for (int i = 0; i < new->count; i++) {
        process_t *curr = &new->procs[i];
        process_t *prev = find_process(old, curr->pid);

        if (!prev) {
            printf("[+] NEW  PID=%d NAME=%s\n",
                   curr->pid, curr->name);
        } else if (strcmp(prev->name, curr->name) != 0) {
            printf("[!] PID REUSE PID=%d OLD=%s NEW=%s\n",
                   curr->pid, prev->name, curr->name);
        }
    }

    /* Exited processes */
    for (int i = 0; i < old->count; i++) {
        process_t *p = &old->procs[i];
        if (!find_process(new, p->pid)) {
            printf("[-] EXIT PID=%d NAME=%s\n",
                   p->pid, p->name);
        }
    }
}

