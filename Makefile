CC = gcc
CFLAGS = -Wall -Werror -std=c23
BUILD_TYPE = Debug
BIN = $(BUILD_DIR)/wcswidth
OBJ = $(BUILD_DIR)/wcswidth.o

ifeq ($(BUILD_TYPE),Debug)
	BUILD_DIR = build/debug
	CFLAGS = -g
else
ifeq ($(BUILD_TYPE),Release)
	BUILD_DIR = build/release
	CFLAGS = -O2
else
	$(error Illegal BUILD_TYPE=$(BUILD_TYPE))
endif
endif

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -o $@ $^ -c

clean:
	rm -v $(BIN) $(OBJ)
