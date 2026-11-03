#include <wunix/registry.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

static int format_val_path(const char *key, const char *val, char *out, size_t out_len) {
    if (!key || !val || !out || out_len < 32) {
        return -1;
    }

    const char *prefix = "/win/registry";
    const char *subkey = key;

    if (strncmp(key, "HKLM\\", 5) == 0 || strncmp(key, "HKLM/", 5) == 0) {
        prefix = "/win/registry/HKLM";
        subkey = key + 5;
    } else if (strncmp(key, "HKEY_LOCAL_MACHINE\\", 19) == 0 || strncmp(key, "HKEY_LOCAL_MACHINE/", 19) == 0) {
        prefix = "/win/registry/HKLM";
        subkey = key + 19;
    } else if (strncmp(key, "HKCU\\", 5) == 0 || strncmp(key, "HKCU/", 5) == 0) {
        prefix = "/win/registry/HKCU";
        subkey = key + 5;
    } else if (strncmp(key, "HKEY_CURRENT_USER\\", 18) == 0 || strncmp(key, "HKEY_CURRENT_USER/", 18) == 0) {
        prefix = "/win/registry/HKCU";
        subkey = key + 18;
    }

    int n = snprintf(out, out_len, "%s/%s/%s", prefix, subkey, val);
    if (n < 0 || (size_t)n >= out_len) {
        return -1;
    }

    // Заменяем обратные слэши на прямые для VFS
    for (size_t i = 0; i < (size_t)n; i++) {
        if (out[i] == '\\') {
            out[i] = '/';
        }
    }
    return 0;
}

int wunix_reg_query_sz(const char *key_path, const char *val_name, char *buf, size_t buf_len) {
    char path[256];
    if (format_val_path(key_path, val_name, path, sizeof(path)) != 0) {
        return -1;
    }

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    uint8_t raw[512];
    ssize_t bytes_read = read(fd, raw, sizeof(raw));
    close(fd);

    if (bytes_read < 4) {
        return -1;
    }

    uint32_t type_id = (uint32_t)raw[0] | ((uint32_t)raw[1] << 8) | ((uint32_t)raw[2] << 16) | ((uint32_t)raw[3] << 24);
    if (type_id != WUNIX_REG_SZ && type_id != WUNIX_REG_EXPAND_SZ) {
        return -1;
    }

    size_t payload_len = (size_t)(bytes_read - 4);
    if (payload_len >= buf_len) {
        return -1;
    }

    memcpy(buf, raw + 4, payload_len);
    buf[payload_len] = '\0';
    return 0;
}

int wunix_reg_query_dword(const char *key_path, const char *val_name, uint32_t *out_val) {
    char path[256];
    if (format_val_path(key_path, val_name, path, sizeof(path)) != 0) {
        return -1;
    }

    int fd = open(path, O_RDONLY);
    if (fd < 0) {
        return -1;
    }

    uint8_t raw[8];
    ssize_t bytes_read = read(fd, raw, sizeof(raw));
    close(fd);

    if (bytes_read < 8) {
        return -1;
    }

    uint32_t type_id = (uint32_t)raw[0] | ((uint32_t)raw[1] << 8) | ((uint32_t)raw[2] << 16) | ((uint32_t)raw[3] << 24);
    if (type_id != WUNIX_REG_DWORD) {
        return -1;
    }

    if (out_val) {
        *out_val = (uint32_t)raw[4] | ((uint32_t)raw[5] << 8) | ((uint32_t)raw[6] << 16) | ((uint32_t)raw[7] << 24);
    }
    return 0;
}
