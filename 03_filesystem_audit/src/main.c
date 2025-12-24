#include <stdio.h>
#include "audit.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory>\n", argv[0]);
        return 1;
    }

    audit_path(argv[1]);
    return 0;
}

