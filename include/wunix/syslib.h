#ifndef _WUNIX_SYSLIB_H
#define _WUNIX_SYSLIB_H

#include <stdint.h>
#include <stddef.h>

#define WUNIX_SYSLIB_INFO_VADDR 0x00007FFFFFFF0000ULL
#define WUNIX_MAX_SYSLIB_SYMBOLS 40

typedef struct {
    uint8_t  name[32];
    uint64_t vaddr;
    uint32_t flags;
    uint32_t reserved;
} wunix_syslib_symbol_t;

typedef struct {
    uint8_t               magic[8];
    uint32_t              version;
    uint32_t              num_symbols;
    uint32_t              page_size;
    uint32_t              ticks_per_sec;
    uint64_t              boot_epoch_sec;
    uint64_t              reserved[2];
    wunix_syslib_symbol_t symbols[WUNIX_MAX_SYSLIB_SYMBOLS];
} wunix_syslib_info_t;

int wunix_syslib_is_available(void);
const wunix_syslib_info_t *wunix_syslib_get_info(void);
uint64_t wunix_syslib_resolve(const char *symbol_name);
uint64_t wunix_syslib_boot_timestamp(void);

size_t wunix_fast_strlen(const char *s);
void  *wunix_fast_memcpy(void *dst, const void *src, size_t n);
void  *wunix_fast_memset(void *dst, int val, size_t n);

#endif /* _WUNIX_SYSLIB_H */
