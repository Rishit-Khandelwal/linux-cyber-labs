#include <stdio.h>
#include "hardening.h"

int main(void) {
    printf("=== Linux Hardening Audit ===\n\n");

    printf("Filesystem checks:\n");
    check_world_writable("/etc");
    check_suid("/usr/bin");

    check_kernel_settings();

    printf("\nAudit complete.\n");
    return 0;
}

