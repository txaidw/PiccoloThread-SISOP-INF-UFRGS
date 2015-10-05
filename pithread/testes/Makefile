#
# Makefile para programas exemplo
# 

CC=gcc
LIB_DIR=../lib

all: teste_mutex

teste_mutex: teste_mutex.c $(LIB_DIR)/libpithread.a
	$(CC) -o teste_mutex teste_mutex.c -L$(LIB_DIR) -lpithread -Wall

clean:
	rm -rf teste_mutex *~