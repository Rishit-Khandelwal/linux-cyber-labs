#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include "hardening.h"

static void audit_permissions(const char *path, const struct stat *st) {
    if (st->st_mode & S_IWOTH) {
        printf("[!] World-writable: %s\n", path);
    }

    if (st->st_mode & S_ISUID) {
        printf("[!] SUID binary: %s\n", path);
    }
}

void check_world_writable(const char *path) {
    struct stat st;
    if (lstat(path, &st) < 0)
        return;

    audit_permissions(path, &st);

    if (!S_ISDIR(st.st_mode))
        return;

    DIR *dir = opendir(path);
    if (!dir)
        return;

    struct dirent *entry;
    char fullpath[512];

    while ((entry = readdir(dir)) != NULL) {
        if (!strcmp(entry->d_name, ".") ||
            !strcmp(entry->d_name, ".."))
            continue;

        int n = snprintf(fullpath, sizeof(fullpath),
                         "%s/%s", path, entry->d_name);
        if (n < 0 || n >= (int)sizeof(fullpath))
            continue;

        check_world_writable(fullpath);
    }

    closedir(dir);
}

void check_suid(const char *path) {
    check_world_writable(path);
}

static void read_sysctl(const char *key) {
    char path[256];
    char value[64];

    snprintf(path, sizeof(path), "/proc/sys/%s", key);
    for (char *p = path; *p; p++) {
        if (*p == '.')
            *p = '/';
    }

    FILE *fp = fopen(path, "r");
    if (!fp)
        return;

    if (fgets(value, sizeof(value), fp)) {
        value[strcspn(value, "\n")] = '\0';
        printf("[*] %s = %s\n", key, value);
    }

    fclose(fp);
}

void check_kernel_settings(void) {
    printf("\nKernel hardening checks:\n");

    read_sysctl("kernel.randomize_va_space");
    read_sysctl("kernel.kptr_restrict");
    read_sysctl("fs.protected_hardlinks");
    read_sysctl("fs.protected_symlinks");
}

