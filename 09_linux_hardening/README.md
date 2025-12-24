# Linux Hardening Audit Tool

A read-only Linux system hardening audit utility written in C.

## What it does
- Scans critical system paths for insecure permissions
- Identifies SUID binaries
- Inspects kernel hardening sysctls

## Why read-only
The tool performs no modifications. This mirrors real security audits
where systems are evaluated before enforcement.

## Security relevance
- World-writable files enable privilege escalation
- SUID binaries expand attack surface
- Kernel hardening reduces exploit reliability

## Limitations
- No automatic remediation
- Limited to visible filesystem paths
- Requires root for full visibility

