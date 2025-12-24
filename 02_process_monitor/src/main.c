#include <unistd.h>
#include "monitor.h"

int main(void) {
    snapshot_t prev, curr;

    take_snapshot(&prev);

    while (1) {
        sleep(2);
        take_snapshot(&curr);
        compare_snapshots(&prev, &curr);
        prev = curr;
    }

    return 0;
}

