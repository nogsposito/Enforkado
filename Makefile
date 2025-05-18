# Nome do executável e pasta de saída
TARGET = Enforkado
BIN_DIR = bin
SRC_DIR = src

# Arquivo principal
SRC = $(SRC_DIR)/main.c

# Compilador e flags
CC = gcc
CFLAGS = -Wall -Wextra -O2
LDFLAGS = -lraylib -lcurl -lGL -lm -lpthread -ldl -lrt -lX11

# Caminho completo do executável
OUT = $(BIN_DIR)/$(TARGET)

.PHONY: all run clean

all: $(OUT)

$(OUT): $(SRC)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

run: all
	./$(OUT)

clean:
	rm -f $(BIN_DIR)/$(TARGET)
