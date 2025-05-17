CC = gcc
SRC = src/main.c
BIN = jogo
CFLAGS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11 -lcurl

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(SRC) -o $(BIN) $(CFLAGS)

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN)
