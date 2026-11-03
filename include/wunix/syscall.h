#ifndef _WUNIX_SYSCALL_H
#define _WUNIX_SYSCALL_H

#include <stdint.h>

#define WUNIX_SYS_READ           0
#define WUNIX_SYS_WRITE          1
#define WUNIX_SYS_OPEN           2
#define WUNIX_SYS_CLOSE          3
#define WUNIX_SYS_STAT           4
#define WUNIX_SYS_FSTAT          5
#define WUNIX_SYS_LSTAT          6
#define WUNIX_SYS_POLL           7
#define WUNIX_SYS_LSEEK          8
#define WUNIX_SYS_MMAP           9
#define WUNIX_SYS_MPROTECT       10
#define WUNIX_SYS_MUNMAP         11
#define WUNIX_SYS_BRK            12
#define WUNIX_SYS_RT_SIGACTION   13
#define WUNIX_SYS_RT_SIGPROCMASK 14
#define WUNIX_SYS_RT_SIGRETURN   15
#define WUNIX_SYS_IOCTL          16
#define WUNIX_SYS_PREAD64        17
#define WUNIX_SYS_PWRITE64       18
#define WUNIX_SYS_READV          19
#define WUNIX_SYS_WRITEV         20
#define WUNIX_SYS_ACCESS         21
#define WUNIX_SYS_PIPE           22
#define WUNIX_SYS_SELECT         23
#define WUNIX_SYS_YIELD          24
#define WUNIX_SYS_NANOSLEEP      35
#define WUNIX_SYS_GETPID         39
#define WUNIX_SYS_SOCKET         41
#define WUNIX_SYS_CONNECT        42
#define WUNIX_SYS_ACCEPT         43
#define WUNIX_SYS_SENDTO         44
#define WUNIX_SYS_RECVFROM       45
#define WUNIX_SYS_SHUTDOWN       48
#define WUNIX_SYS_BIND           49
#define WUNIX_SYS_LISTEN         50
#define WUNIX_SYS_GETSOCKNAME    51
#define WUNIX_SYS_GETPEERNAME    52
#define WUNIX_SYS_SETSOCKOPT     54
#define WUNIX_SYS_GETSOCKOPT     55
#define WUNIX_SYS_CLONE          56
#define WUNIX_SYS_EXECVE         59
#define WUNIX_SYS_EXIT           60
#define WUNIX_SYS_WAIT4          61
#define WUNIX_SYS_KILL           62
#define WUNIX_SYS_UNAME          63
#define WUNIX_SYS_FCNTL          72
#define WUNIX_SYS_FLOCK          73
#define WUNIX_SYS_FSYNC          74
#define WUNIX_SYS_FDATASYNC      75
#define WUNIX_SYS_GETCWD         79
#define WUNIX_SYS_CHDIR          80
#define WUNIX_SYS_FCHDIR         81
#define WUNIX_SYS_GETTIMEOFDAY   96
#define WUNIX_SYS_GETRLIMIT      97
#define WUNIX_SYS_GETRUSAGE      98
#define WUNIX_SYS_SYSINFO        99
#define WUNIX_SYS_TIMES          100
#define WUNIX_SYS_GETUID         102
#define WUNIX_SYS_GETGID         104
#define WUNIX_SYS_SETUID         105
#define WUNIX_SYS_SETGID         106
#define WUNIX_SYS_GETEUID        107
#define WUNIX_SYS_GETEGID        108
#define WUNIX_SYS_GETGROUPS      115
#define WUNIX_SYS_SETGROUPS      116
#define WUNIX_SYS_ARCH_PRCTL     158
#define WUNIX_SYS_SETRLIMIT      160
#define WUNIX_SYS_SYNC           162
#define WUNIX_SYS_REBOOT         169
#define WUNIX_SYS_FUTEX          202
#define WUNIX_SYS_GETDENTS64     217
#define WUNIX_SYS_CLOCK_GETTIME  228
#define WUNIX_SYS_EXIT_GROUP     231
#define WUNIX_SYS_OPENAT         257
#define WUNIX_SYS_NEWFSTATAT     262
#define WUNIX_SYS_PSELECT6       270
#define WUNIX_SYS_PPOLL          271
#define WUNIX_SYS_UTIMENSAT      280
#define WUNIX_SYS_PIPE2          293
#define WUNIX_SYS_PRLIMIT64      302
#define WUNIX_SYS_GETRANDOM      318

static inline int64_t wunix_syscall0(uint64_t nr) {
    int64_t ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(nr)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t wunix_syscall1(uint64_t nr, uint64_t arg1) {
    int64_t ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(nr), "D"(arg1)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t wunix_syscall2(uint64_t nr, uint64_t arg1, uint64_t arg2) {
    int64_t ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(nr), "D"(arg1), "S"(arg2)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t wunix_syscall3(uint64_t nr, uint64_t arg1, uint64_t arg2, uint64_t arg3) {
    int64_t ret;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(nr), "D"(arg1), "S"(arg2), "d"(arg3)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t wunix_syscall4(uint64_t nr, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4) {
    int64_t ret;
    register uint64_t r10 __asm__("r10") = arg4;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(nr), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t wunix_syscall5(uint64_t nr, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5) {
    int64_t ret;
    register uint64_t r10 __asm__("r10") = arg4;
    register uint64_t r8 __asm__("r8") = arg5;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(nr), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10), "r"(r8)
        : "rcx", "r11", "memory"
    );
    return ret;
}

static inline int64_t wunix_syscall6(uint64_t nr, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
    int64_t ret;
    register uint64_t r10 __asm__("r10") = arg4;
    register uint64_t r8 __asm__("r8") = arg5;
    register uint64_t r9 __asm__("r9") = arg6;
    __asm__ __volatile__(
        "syscall"
        : "=a"(ret)
        : "a"(nr), "D"(arg1), "S"(arg2), "d"(arg3), "r"(r10), "r"(r8), "r"(r9)
        : "rcx", "r11", "memory"
    );
    return ret;
}

#endif /* _WUNIX_SYSCALL_H */
