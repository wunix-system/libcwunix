#include <wunix/syslib.h>
#include <string.h>

static const uint8_t SYSLIB_MAGIC[8] = {'W', 'U', 'N', 'I', 'X', 'L', 'I', 'B'};

int wunix_syslib_is_available(void) {
    const wunix_syslib_info_t *info = (const wunix_syslib_info_t *)WUNIX_SYSLIB_INFO_VADDR;
    for (int i = 0; i < 8; i++) {
        if (info->magic[i] != SYSLIB_MAGIC[i]) {
            return 0;
        }
    }
    return 1;
}

const wunix_syslib_info_t *wunix_syslib_get_info(void) {
    if (wunix_syslib_is_available()) {
        return (const wunix_syslib_info_t *)WUNIX_SYSLIB_INFO_VADDR;
    }
    return NULL;
}

uint64_t wunix_syslib_resolve(const char *symbol_name) {
    const wunix_syslib_info_t *info = wunix_syslib_get_info();
    if (!info || !symbol_name) {
        return 0;
    }

    size_t query_len = strlen(symbol_name);
    uint32_t count = info->num_symbols;
    if (count > WUNIX_MAX_SYSLIB_SYMBOLS) {
        count = WUNIX_MAX_SYSLIB_SYMBOLS;
    }

    for (uint32_t i = 0; i < count; i++) {
        size_t len = 0;
        while (len < 32 && info->symbols[i].name[len] != 0) {
            len++;
        }
        if (len == query_len && memcmp(info->symbols[i].name, symbol_name, len) == 0) {
            return info->symbols[i].vaddr;
        }
    }
    return 0;
}

uint64_t wunix_syslib_boot_timestamp(void) {
    const wunix_syslib_info_t *info = wunix_syslib_get_info();
    return info ? info->boot_epoch_sec : 0;
}

size_t wunix_fast_strlen(const char *s) {
    typedef size_t (*strlen_fn)(const char *);
    uint64_t addr = wunix_syslib_resolve("strlen");
    if (addr) {
        return ((strlen_fn)addr)(s);
    }
    return strlen(s);
}

void *wunix_fast_memcpy(void *dst, const void *src, size_t n) {
    typedef void *(*memcpy_fn)(void *, const void *, size_t);
    uint64_t addr = wunix_syslib_resolve("memcpy");
    if (addr) {
        return ((memcpy_fn)addr)(dst, src, n);
    }
    return memcpy(dst, src, n);
}

void *wunix_fast_memset(void *dst, int val, size_t n) {
    typedef void *(*memset_fn)(void *, int, size_t);
    uint64_t addr = wunix_syslib_resolve("memset");
    if (addr) {
        return ((memset_fn)addr)(dst, val, n);
    }
    return memset(dst, val, n);
}
