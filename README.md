# libcwunix

> Стандартная библиотека языка C (libc) для пространства пользователя (Ring 3) операционной системы Wunix.

## Описание

`libcwunix` — официальная реализация стандартной библиотеки C (libc) для пространства пользователя Ring 3 операционной системы Wunix.

Библиотека обеспечивает:
1. **Полный стандартный API POSIX и C11**: стандартный ввод-вывод (`stdio.h`), управление памятью (`stdlib.h`, `malloc`, `free`), строковые операции (`string.h`), математика (`math.h`, `libm`), сокеты Беркли (`sys/socket.h`), потоки POSIX (`pthread.h`), управление терминалом (`termios.h`) и сигналами (`signal.h`).
2. **Прямую совместимость с Linux x86_64 Syscall ABI**: системные вызовы напрямую взаимодействуют с монолитным ядром Wunix через процессорную инструкцию `syscall`.
3. **Нативные расширения Wunix (`<wunix/wunix.h>`)**:
   - `<wunix/syscall.h>` — прямые обёртки системных вызовов (`wunix_syscall0` .. `wunix_syscall6`) и именованные константы сисколлов Wunix.
   - `<wunix/syslib.h>` — интеграция со страницей vDSO (`0x00007FFFFFFF0000`): быстрый доступ к меткам времени загрузки, системным параметрам и оптимизированным функциям памяти (`wunix_fast_strlen`, `wunix_fast_memcpy`, `wunix_fast_memset`).

### Структура каталога

```
libs/libcwunix/
├── include/              # Стандартные заголовки POSIX/C и <wunix/...>
│   ├── stdio.h, stdlib.h, unistd.h, termios.h, string.h, ...
│   ├── sys/, arpa/, netinet/, bits/
│   └── wunix/
│       ├── syscall.h     # Номера и макросы сисколлов Wunix
│       ├── syslib.h      # vDSO интерфейс
│       └── wunix.h       # Единый зонтичный заголовок
├── lib/                  # Статические библиотеки и стартап-объекты
│   ├── crt1.o, crti.o, crtn.o   # Инициализация и финализация процесса (C runtime)
│   ├── libcwunix.a              # Объединенная статическая библиотека libc + Wunix
│   └── libc.a, libm.a, ...      # Симлинки на libcwunix.a
├── src/                  # Исходный код нативных расширений Wunix
│   └── wunix_syslib.c    # Реализация vDSO резолвера
├── Makefile              # Сборка расширений и упаковка в libcwunix.a
└── wunix-cc              # Автономный скрипт компиляции C-программ под Wunix
```

## Компиляция программ для Wunix

### Способ 1. Использование встроенного компилятора `wunix-cc`

В состав репозитория `libcwunix` входит автономный скрипт-обёртка `wunix-cc`, который автоматически настраивает пути к заголовкам и стартап-объектам:

```bash
# Компиляция статического бинарника формата ELF64
./wunix-cc -O2 main.c -o myprog

# Сборка с дополнительными флагами
./wunix-cc -O2 -Wall -Wextra app.c -o app
```

Скрипт `wunix-cc` автоматически:
- Отключает стандартные заголовочные файлы хостовой операционной системы (`-nostdinc -isystem include`).
- Подключает заголовочные файлы `include/` и изолирует libc.
- Выполняет статическую линковку (`-static -nostdlib`) со стартап-кодом `lib/crt1.o`, `lib/crti.o`, библиотекой `lib/libcwunix.a` и `lib/crtn.o`.

### Способ 2. Прямой вызов GCC (для Makefile или CMake)

Если вы интегрируете `libcwunix` в собственную сборочную систему, используйте стандартные флаги GCC:

```bash
gcc -nostdinc -isystem /путь/к/libcwunix/include -nostdlib -static \
    /путь/к/libcwunix/lib/crt1.o /путь/к/libcwunix/lib/crti.o \
    main.c \
    -L/путь/к/libcwunix/lib -lcwunix -lgcc \
    /путь/к/libcwunix/lib/crtn.o \
    -o myprog
```

Скомпилированный файл представляет собой автономный статический исполняемый файл формата ELF64, готовый к запуску в ядре Wunix.

## Пример программы на C

```c
#include <stdio.h>
#include <wunix/wunix.h>

int main(int argc, char **argv) {
    (void)argc;
    (void)argv;

    printf("Программа успешно запущена в операционной системе %s!\n", WUNIX_OS_NAME);

    // Получение времени загрузки ядра через страницу vDSO
    uint64_t boot_ts = wunix_syslib_boot_timestamp();
    printf("Метка времени загрузки ядра (vDSO): %llu сек\n", (unsigned long long)boot_ts);

    return 0;
}
```

## Статус реализации

- **Реализовано**: базовый рантайм POSIX/C11, статическая компоновка через автономный `wunix-cc` и флаги GCC, обертки системных вызовов Linux Syscall ABI, vDSO-акселерация, интеграция с портированными программами (TinyCC, OpenSSL, OpenSSH).
- **Не реализовано**: динамический компоновщик ELF64 (`ld.so`) и разделяемые библиотеки (`.so`) — запланированы в будущих версиях Wunix.

## Сборка

Для сборки библиотеки `libcwunix.a` используется стандартная утилита Make:

```bash
make clean
make
```

Собранная статическая библиотека размещается по пути:
`lib/libcwunix.a`
