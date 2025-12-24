#ifndef MONITOR_H
#define MONITOR_H

#include <time.h>

#define MAX_PROCS 32768

typedef struct {
    int pid;
    char name[64];
    time_t first_seen;
} process_t;

typedef struct {
    process_t procs[MAX_PROCS];
    int count;
} snapshot_t;

void take_snapshot(snapshot_t *snap);
void compare_snapshots(snapshot_t *old, snapshot_t *new);

#endif

