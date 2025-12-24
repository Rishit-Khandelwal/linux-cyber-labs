# Filesystem Permission & SUID Audit

A Linux filesystem security audit tool written in C.

## What it does
- Recursively scans directories
- Detects world-writable files and directories
- Detects SUID and SGID binaries

## How it works
The tool traverses the filesystem using `lstat()` and inspects
permission bits exposed by the kernel. Symlinks are not followed to
avoid traversal attacks.

## Security relevance
- World-writable files enable privilege escalation
- SUID/SGID binaries execute with elevated privileges
- Misconfigurations are common real-world vulnerabilities

## Limitations
- No pruning of trusted paths
- No ownership or hash verification
- Recursive DFS may be slow on large filesystems

