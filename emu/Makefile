CC = gcc
CFLAGS = -std=c17 -Wall -Wextra -Wpedantic -O2 -MMD -MP -Iinclude
TARGET = cpu32-emu

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:src/%.c=build/%.o)

$(TARGET): $(OBJS)
	$(CC) $^ -o $@

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build

-include $(OBJS:.o=.d)

.PHONY: clean
