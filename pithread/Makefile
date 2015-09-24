#
# Makefile ESQUELETO
#
# OBRIGATÓRIO ter uma regra "all" para geração da biblioteca e de uma
# regra "clean" para remover todos os objetos gerados.
#
# NECESSARIO adaptar este esqueleto de makefile para suas necessidades.
#
# OBSERVAR que as variáveis de ambiente consideram que o Makefile está no diretótio "pithread"
#

CC=gcc
LIB_DIR=./lib
INC_DIR=./include
BIN_DIR=./bin
SRC_DIR=./src

all: pidata.o pithread.o queue_handler.o threads_persistence.o 	libpithread

onlyobj: pidata.o pithread.o queue_handler.o threads_persistence.o

pidata.o: $(SRC_DIR)/pidata.c
	$(CC) -c $(SRC_DIR)/pidata.c -o $(BIN_DIR)/pidata.o -Wall

pithread.o: $(SRC_DIR)/pithread.c
	$(CC) -c $(SRC_DIR)/pithread.c -o $(BIN_DIR)/pithread.o -Wall

queue_handler.o: $(SRC_DIR)/queue_handler.c
	$(CC) -c $(SRC_DIR)/queue_handler.c -o $(BIN_DIR)/queue_handler.o -Wall

threads_persistence.o: $(SRC_DIR)/threads_persistence.c
	$(CC) -c $(SRC_DIR)/threads_persistence.c -o $(BIN_DIR)/threads_persistence.o -Wall

libpithread: $(BIN_DIR)/pidata.o $(BIN_DIR)/pithread.o
	ar crs $(LIB_DIR)/libpithread.a $(BIN_DIR)/pidata.o $(BIN_DIR)/pithread.o

clean:
	rm -rf $(LIB_DIR)/*.a $(BIN_DIR)/*.o $(SRC_DIR)/*~ $(INC_DIR)/*~ *~
