# SSH Honeypot

A minimal SSH honeypot written in C.

## What it does
- Listens for SSH connections
- Emulates an SSH banner
- Captures login credentials
- Logs attacker IP and timestamps

## How it works
The honeypot listens on a TCP port and mimics basic SSH interaction.
Automated scanners and brute-force tools often submit credentials
without verifying full SSH protocol compliance.

## Security relevance
- Detects brute-force attempts
- Captures attacker behavior
- Demonstrates deception-based defense

## Safety
- No real authentication
- No shell access
- Runs without root

## Limitations
- Not a full SSH implementation
- IPv4 only
- Sequential handling


