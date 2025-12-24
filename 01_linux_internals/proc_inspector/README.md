# proc_inspector

A Linux system introspection tool written in C that reads kernel state via `/proc`.

## What this project demonstrates
- Kernel to user-space data exposure
- Process metadata inspection
- Direct use of system calls

## Data sources
- `/proc/cpuinfo`
- `/proc/meminfo`
- `/proc/[pid]/status`

## Security relevance
Process enumeration and memory inspection form the foundation of
host-based intrusion detection and system monitoring.

