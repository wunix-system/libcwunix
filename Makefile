# Makefile — Сборка системной библиотеки libcwunix для Wunix OS
CC ?= gcc
AR ?= ar
RANLIB ?= ranlib

LIBDIR := lib
INCDIR := include
SRCDIR := src

CFLAGS ?= -O2 -Wall -Wextra -fno-stack-protector -nostdinc -isystem $(INCDIR)

OBJS := $(SRCDIR)/wunix_syslib.o $(SRCDIR)/wunix_registry.o

all: $(LIBDIR)/libcwunix.a

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBDIR)/libcwunix.a: $(OBJS)
	@echo "==> Интеграция Wunix-расширений в libcwunix.a..."
	$(AR) rcs $@ $(OBJS)
	$(RANLIB) $@
	@echo "==> Библиотека libcwunix.a успешно собрана!"

clean:
	rm -f $(SRCDIR)/*.o

.PHONY: all clean
