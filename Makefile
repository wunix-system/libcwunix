# Makefile — Сборка системной библиотеки libcwunix для Wunix OS
CC ?= gcc
AR ?= ar
RANLIB ?= ranlib

LIBDIR := lib
INCDIR := include
SRCDIR := src

CFLAGS ?= -O2 -Wall -Wextra -fno-stack-protector -nostdinc -isystem $(INCDIR)

OBJS := $(SRCDIR)/wunix_syslib.o $(SRCDIR)/wunix_registry.o

SO_LINKS := $(LIBDIR)/libc.so $(LIBDIR)/libm.so $(LIBDIR)/libdl.so $(LIBDIR)/libpthread.so $(LIBDIR)/librt.so $(LIBDIR)/libutil.so

all: $(LIBDIR)/libcwunix.a $(LIBDIR)/libcwunix.so $(SO_LINKS)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -fPIC -c $< -o $@

$(LIBDIR)/libcwunix.a: $(OBJS)
	@echo "==> Интеграция Wunix-расширений в libcwunix.a..."
	$(AR) rcs $@ $(OBJS)
	$(RANLIB) $@
	@echo "==> Библиотека libcwunix.a успешно собрана!"

$(LIBDIR)/libcwunix.so: $(LIBDIR)/libcwunix.a
	@echo "==> Сборка динамической библиотеки libcwunix.so..."
	$(CC) -shared -nostdlib -Wl,-soname,libcwunix.so -Wl,--whole-archive $(LIBDIR)/libcwunix.a -Wl,--no-whole-archive -lgcc -o $@
	@echo "==> Динамическая библиотека libcwunix.so успешно собрана!"

$(LIBDIR)/lib%.so: $(LIBDIR)/libcwunix.so
	ln -sf libcwunix.so $@

$(LIBDIR)/libc.so: $(LIBDIR)/libcwunix.so
	ln -sf libcwunix.so $@

clean:
	rm -f $(SRCDIR)/*.o

.PHONY: all clean
