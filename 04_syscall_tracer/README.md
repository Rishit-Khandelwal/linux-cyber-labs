# Syscall Tracer (ptrace)

A minimal syscall tracing tool written in C using `ptrace`.

## What it does
- Traces system calls of a target process
- Prints syscall numbers and return values
- Demonstrates user → kernel boundary inspection

## How it works
The tracer forks a child process, enables tracing with `PTRACE_TRACEME`,
and intercepts system call entry and exit using `PTRACE_SYSCALL`.
Registers are inspected via `PTRACE_GETREGS`.

## Security relevance
- All malicious actions require syscalls
- Syscall tracing enables behavior-based detection
- This is the foundation of debuggers, sandboxes, and EDR agents

## Limitations
- x86-64 specific
- No syscall name resolution
- No argument decoding

