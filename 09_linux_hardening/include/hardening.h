#ifndef HARDENING_H
#define HARDENING_H

void check_world_writable(const char *path);
void check_suid(const char *path);
void check_kernel_settings(void);

#endif

