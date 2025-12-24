# Authentication Log Analyzer

A portable Linux authentication log analyzer written in C.

## What it does
- Locates system authentication logs across distributions
- Parses failed login attempts
- Aggregates failures by source IP
- Flags suspicious IPs

## Supported log locations
- /var/log/auth.log (Debian/Ubuntu)
- /var/log/secure (RHEL/CentOS)

## Security relevance
- Detects brute-force login attempts
- Demonstrates log-based intrusion detection
- Mirrors SOC-style analysis workflows

## Limitations
- IPv4 only
- Journal-based systems not supported
- Sequential parsing

