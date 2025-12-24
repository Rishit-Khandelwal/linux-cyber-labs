# Rate Limiter & Defense Tool

A simple IP-based rate-limiting server written in C.

## What it does
- Accepts TCP connections
- Enforces per-IP request limits
- Blocks abusive clients
- Logs allowed and blocked requests

## How it works
Each client IP is tracked in a fixed-size table. Requests are counted
within a time window. When the limit is exceeded, connections are dropped.

## Security relevance
- Prevents brute-force attacks
- Mitigates denial-of-service attempts
- Demonstrates active defense logic

## Limitations
- IPv4 only
- No persistence across restarts
- Linear lookup

