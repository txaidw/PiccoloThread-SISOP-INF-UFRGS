#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib

all: teste1 teste2

teste1: teste1.c $(LIB_DIR)/libpithread.a
	$(CC) -o teste1 teste1.c -L$(LIB_DIR) -lpithread -Wall

teste2: teste2.c $(LIB_DIR)/libpithread.a
	$(CC) -o teste2 teste2.c -L$(LIB_DIR) -lpithread -Wall

clean:
	rm -rf teste1 teste2 *~