#ifndef _WUNIX_REGISTRY_H
#define _WUNIX_REGISTRY_H

#include <stdint.h>
#include <stddef.h>

#define WUNIX_REG_NONE      0
#define WUNIX_REG_SZ        1
#define WUNIX_REG_EXPAND_SZ 2
#define WUNIX_REG_BINARY    3
#define WUNIX_REG_DWORD     4
#define WUNIX_REG_QWORD     11

int wunix_reg_query_sz(const char *key_path, const char *val_name, char *buf, size_t buf_len);
int wunix_reg_query_dword(const char *key_path, const char *val_name, uint32_t *out_val);

#endif /* _WUNIX_REGISTRY_H */
