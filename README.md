# Linux Cyber Labs — Systems & Security Projects in C

This repository contains a series of progressively advanced Linux systems
and cybersecurity projects implemented in **pure C**, without frameworks
or external tooling.

The goal of this repository is to demonstrate **how Linux actually works**
under the hood — from kernel-exposed state and process lifecycle, to syscall
boundaries, networking, detection, and active defense.

This is **not** a collection of tool wrappers.
Every project is built from first principles.

---

## Project Overview

### 1. Linux Internals — Process Introspection
**Path:** `01_linux_internals/proc_inspector`

- Reads kernel state via `/proc`
- Enumerates processes
- Parses `/proc/[pid]/status` and `/proc/[pid]/stat`
- Demonstrates kernel → user-space data exposure

---

### 2. Process Lifecycle Monitor
**Path:** `02_process_monitor`

- Polls `/proc` to detect:
  - new processes
  - exited processes
  - PID reuse
- Models process behavior over time
- Foundation for host-based detection systems

---

### 3. Filesystem Permission & SUID Audit
**Path:** `03_filesystem_audit`

- Recursively audits filesystem permissions
- Detects:
  - world-writable files/directories
  - SUID / SGID binaries
- Uses `lstat()` to avoid symlink traversal
- Mirrors real hardening and audit workflows

---

### 4. Syscall Tracer (ptrace)
**Path:** `04_syscall_tracer`

- Minimal `strace`-like syscall tracer
- Uses `ptrace` to intercept syscall entry/exit
- Reads registers directly (`orig_rax`, `rax`)
- Demonstrates user ↔ kernel boundary inspection

---

### 5. TCP Port Scanner
**Path:** `05_tcp_port_scanner`

- TCP connect() scan implementation
- Non-blocking sockets + `select()` timeouts
- Correct handling of:
  - open ports
  - closed ports
  - filtered ports
- Shows how scanners work internally

---

### 6. Authentication Log Analyzer
**Path:** `06_auth_log_analyzer`

- Portable auth log discovery across distros
- Parses failed login attempts
- Aggregates failures by IP
- Demonstrates SOC-style log analysis

---

### 7. SSH Honeypot
**Path:** `07_ssh_honeypot`

- Fake SSH service (safe, user-space)
- Captures:
  - source IP
  - usernames
  - passwords
- Designed to attract automated brute-force tools
- Demonstrates deception-based defense

---

### 8. Rate Limiter & Defense Tool
**Path:** `08_rate_limiter`

- IP-based rate limiting server
- Sliding time window enforcement
- Actively blocks abusive clients
- Models real-world defensive logic (WAF / SSH protection)

---

### 9. Linux Hardening Audit Tool
**Path:** `09_linux_hardening`

- Read-only system hardening audit
- Checks:
  - dangerous permissions
  - SUID binaries
  - kernel hardening sysctls
- Mirrors CIS-style baseline assessments

---

## Design Principles

- **Pure C**, no frameworks
- Warning-clean builds (`-Wall -Wextra`)
- Kernel-adjacent interfaces
- Read-only where appropriate
- Portable across Linux distributions

---

## Intended Audience

This repository is intended for:
- Linux systems interns
- Security engineering interns
- SOC / blue-team 
- Anyone learning Linux internals seriously

---

## Disclaimer

All tools are for educational and defensive purposes only.
No destructive actions are performed.

