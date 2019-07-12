CC = gcc
CFLAGS = -lpbc -lgmp -lssl -lcrypto -lm

all: comp keyGen invIndGen build search

comp:
	$(CC) -g -Wall  src/aux/*.c src/binv.c  -o binv  -L. -I include $(CFLAGS)
	$(CC) -g -Wall  src/aux/*.c src/vsearch.c -o vsearch  -L. -I include $(CFLAGS)
keyGen:
	./vsearch 1
invIndGen:
	python3 create_inv_index.py
build:
	./vsearch 2
search:
	./vsearch 3
update:
	./vsearch 4
refresh:
	rm vsearch binv
	rm -r data/inv_index
	rm data/secrets data/param.txt  data/signatures
	rm data/auditor-result data/owner-result 
	
