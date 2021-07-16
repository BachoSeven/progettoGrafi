include config.mk

SRC = grafo.c

all: script grafo

script:
	./filtro.sh

grafo:
	${CC} ${SRC} -o ${SRC%.c}

clean:
	rm -rf txt/*.txt grafo

.PHONY: all script grafo clean
