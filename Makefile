ifeq ($(OS),Windows_NT)
	CC = gcc
	TARGET = bin/bomba.exe
	LDFLAGS = -lws2_32
else
	CC = x86_64-w64-mingw32-gcc
	TARGET = bin/bomba.exe
	LDFLAGS = -lws2_32
endif

SRCDIR   = src
INCDIR   = include
BUILDDIR = build
BINDIR   = bin

SRCS = $(wildcard $(SRCDIR)/*.c)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(BUILDDIR)/%.o)

CFLAGS = -Wall -Wextra -std=c11 -I$(INCDIR)

all: $(TARGET)

$(TARGET): $(OBJS)
	mkdir -p $(BINDIR)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(BINDIR)

.PHONY: all clean

