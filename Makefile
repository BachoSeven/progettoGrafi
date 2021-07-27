include config.mk

SRC = grafo.cpp
OBJ = ${SRC:.cpp=}

all: filtro grafo

filtro:
	mkdir -p txt
	./filtro.sh

grafo:
	${CC} -o ${OBJ} ${SRC} ${CFLAGS}
	./grafo

clean:
	rm -rf txt/*.txt grafo

.PHONY: all filtro grafo clean
