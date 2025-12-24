#include <stdio.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>

#include "audit.h"

static void check_permissions(const char *path, const struct stat *st) {
    /* World-writable file or directory */
    if (st->st_mode & S_IWOTH) {
        printf("[!] WORLD-WRITABLE: %s\n", path);
    }

    /* SUID */
    if (st->st_mode & S_ISUID) {
        printf("[!] SUID SET: %s\n", path);
    }

    /* SGID */
    if (st->st_mode & S_ISGID) {
        printf("[!] SGID SET: %s\n", path);
    }
}

void audit_path(const char *path) {
    struct stat st;

    if (lstat(path, &st) < 0)
        return;

    check_permissions(path, &st);

    /* Only recurse into directories */
    if (!S_ISDIR(st.st_mode))
        return;

    DIR *dir = opendir(path);
    if (!dir)
        return;

    struct dirent *entry;
    char fullpath[PATH_MAX];

    while ((entry = readdir(dir)) != NULL) {
        /* Skip . and .. */
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;

        int written = snprintf(fullpath, sizeof(fullpath),
                               "%s/%s", path, entry->d_name);
        if (written < 0 || written >= (int)sizeof(fullpath))
            continue;

        audit_path(fullpath);
    }

    closedir(dir);
}

