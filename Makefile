CC = gcc
CFLAGS = -lpbc -lgmp -lssl -lcrypto -lm

all: keygen build search-cloud search-owner search-auditor update

keygen: utilities.c utilities.h
		$(CC) -o keygen keygen.c utilities.c $(CFLAGS)
build: utilities.c utilities.h
		$(CC) -o build build.c utilities.c $(CFLAGS)
search-owner: utilities.c utilities.h
		$(CC) -o search-owner search-owner.c utilities.c $(CFLAGS)
search-cloud: utilities.c utilities.h search_token.c search_token.h
		$(CC) -o search-cloud search-cloud.c search_token.c utilities.c $(CFLAGS)
search-auditor: utilities.c utilities.h
		$(CC) -o search-auditor search-auditor.c utilities.c $(CFLAGS)
update: utilities.c utilities.h build_inv_index.c
		$(CC) -o update update.c utilities.c build_inv_index.c $(CFLAGS)
