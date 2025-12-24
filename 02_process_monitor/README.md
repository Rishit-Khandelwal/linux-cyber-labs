# Process Lifecycle Monitor

A user-space Linux process monitoring tool written in C.

## What it does
- Polls `/proc` at fixed intervals
- Detects new and exited processes
- Attaches process names
- Detects PID reuse

## How it works
Linux does not notify user-space programs when processes start or exit.
This tool periodically snapshots `/proc` and compares snapshots to infer
process lifecycle events.

## Security relevance
- Detects unauthorized execution
- Highlights process respawning
- Demonstrates polling-based monitoring limitations

## Limitations
- Short-lived processes may be missed
- O(n²) comparison
- No kernel event subscription

