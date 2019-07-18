CC = gcc
CFLAGS = -lpbc -lgmp -lssl -lcrypto -lm 

all: comp init_db #keyGen invIndGen build search

comp:
	$(CC) -g -Wall  src/aux/*.c src/binv.c  -o binv  `mysql_config --cflags --libs` -L -std=c99 -I include $(CFLAGS)
	$(CC) -g -Wall  src/aux/*.c src/vsearch.c -o vsearch  `mysql_config --cflags --libs` -L -std=c99 -I include $(CFLAGS)

init_db:
	$(CC) -g -Wall src/init_db.c -o init_db  `mysql_config --cflags --libs` -L -std=c99 -I include
# keyGen:
# 	./vsearch 1
# invIndGen:
# 	python3 create_inv_index.py
# build:
# 	./vsearch 2
# search:
# 	./vsearch 3
# update:
# 	./vsearch 4
refresh:
	rm init_db
	rm vsearch binv
	rm -r data/inv_index
	rm data/secrets data/param.txt
	rm data/auditor-result data/owner-result
