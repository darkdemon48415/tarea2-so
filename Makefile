CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -D_DEFAULT_SOURCE -pthread
SRC     = main.c config_input.c servidor_matchmaker.c juez_simulacion.c resiliencia.c
BIN     = komikan

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

clean:
	rm -f $(BIN)

.PHONY: clean
