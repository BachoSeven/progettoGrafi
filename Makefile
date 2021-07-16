include config.mk

SRC = grafo.cpp
OBJ = ${SRC:.cpp=}

all: filtro grafo

filtro:
	./filtro.sh

grafo:
	${CC} -o ${OBJ} ${SRC} ${CFLAGS}

clean:
	rm -rf txt/*.txt grafo

.PHONY: all filtro grafo clean
