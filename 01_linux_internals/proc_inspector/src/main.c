#include "../include/proc.h"

int main(void) {
    read_cpuinfo();
    read_meminfo();
    list_processes();
    return 0;
}

