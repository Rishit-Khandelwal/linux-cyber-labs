# TCP Port Scanner

A minimal TCP connect() port scanner written in C.

## What it does
- Scans a range of TCP ports
- Uses non-blocking sockets
- Applies timeouts using select()

## How it works
The scanner initiates a non-blocking TCP connect and waits for the socket
to become writable. The result is verified using SO_ERROR to determine
whether the port is open or filtered.

## Security relevance
- Demonstrates TCP handshake behavior
- Shows how scanners detect exposed services
- Highlights limits of network visibility

## Limitations
- TCP connect scan only
- No SYN scanning
- Sequential (no parallelism)

