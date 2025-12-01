# This makefile rebuilds the test assets.

CHERISDK = ~/cheri/cherisdk
SDK_BIN = $(CHERISDK)/sdk/bin
CC = $(SDK_BIN)/clang
CFLAGS = --sysroot ~/cheri/cherisdk/rootfs-riscv64-purecap \
	-target riscv64-unknown-freebsd \
	-march=rv64imafdcxcheri \
	-mabi=l64pc128d \
	-mno-relax \
	-g -gdwarf-4

SRCS = $(wildcard sample_*.c)
TARGETS = $(patsubst %.c,%,$(SRCS))

.PHONY: all clean

all: $(TARGETS)

%: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)
