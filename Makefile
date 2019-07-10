CC = gcc
CFLAGS = -lpbc -lgmp -lssl -lcrypto -lm

all: vsearch binv

vsearch : utilities.c keygen.c build.c search-cloud.c search-owner.c search-auditor.c update.c search_token.c build_inv_index.c
	$(CC) -o vsearch vsearch.c utilities.c keygen.c build.c search-cloud.c search-owner.c search-auditor.c update.c search_token.c build_inv_index.c $(CFLAGS)

binv : utilities.c keygen.c build.c search-cloud.c search-owner.c search-auditor.c update.c search_token.c build_inv_index.c
	$(CC) -o binv binv.c utilities.c keygen.c build.c search-cloud.c search-owner.c search-auditor.c update.c search_token.c build_inv_index.c $(CFLAGS)
