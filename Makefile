CC = gcc
CFLAGS = -g -Wall -std=c90

ru_na_pandemia: ru_na_pandemia.o libru.o libpilha.o libfila.o liblista.o
	$(CC) ru_na_pandemia.o libru.o libpilha.o libfila.o liblista.o -o ru_na_pandemia

ru_na_pandemia.o: ru_na_pandemia.c
	$(CC) $(CFLAGS) -c ru_na_pandemia.c

libru.o: libru.c
	$(CC) $(CFLAGS) -c libru.c


libpilha.o: libpilha.c
	$(CC) $(CFLAGS) -c libpilha.c

libfila.o: libfila.c
	$(CC) $(CFLAGS) -c libfila.c

liblista.o: liblista.c
	$(CC) $(CFLAGS) -c liblista.c


clean:
	rm *.o ru_na_pandemia
