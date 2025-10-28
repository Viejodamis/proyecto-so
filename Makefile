CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c11
LDFLAGS = -pthread

SRC_DIR = src
BIN_DIR = bin

SRV = $(BIN_DIR)/controlador
CLI = $(BIN_DIR)/agente

SRV_SRC = $(SRC_DIR)/controlador.c
CLI_SRC = $(SRC_DIR)/agente.c

INC = $(SRC_DIR)/comun.h

.PHONY: all clean

all: $(SRV) $(CLI)

$(SRV): $(SRV_SRC) $(INC)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(SRV_SRC) -o $(SRV) $(LDFLAGS)

$(CLI): $(CLI_SRC) $(INC)
	$(CC) $(CFLAGS) -I$(SRC_DIR) $(CLI_SRC) -o $(CLI) $(LDFLAGS)

clean:
	rm -rf $(BIN_DIR)/*
