#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "tracer.h"

void trace_process(char *program, char **args) {
    pid_t child = fork();

    if (child == 0) {
        /* Child: allow tracing */
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(program, args);
        perror("execvp");
        exit(1);
    } else if (child > 0) {
        int status;
        struct user_regs_struct regs;

        /* Wait for exec */
        waitpid(child, &status, 0);

        while (1) {
            /* Stop at syscall entry */
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            waitpid(child, &status, 0);
            if (WIFEXITED(status))
                break;

            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            long syscall = regs.orig_rax;

            /* Stop at syscall exit */
            ptrace(PTRACE_SYSCALL, child, NULL, NULL);
            waitpid(child, &status, 0);
            if (WIFEXITED(status))
                break;

            ptrace(PTRACE_GETREGS, child, NULL, &regs);
            long retval = regs.rax;

            printf("syscall=%ld return=%ld\n", syscall, retval);
        }
    }
}
int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <program> [args...]\n", argv[0]);
        return 1;
    }

    trace_process(argv[1], &argv[1]);
    return 0;
}

