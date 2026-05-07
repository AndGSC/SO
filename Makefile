CC=gcc
CFLAGS=-Wall -Wextra -pthread -Iinclude
TARGET=torneo_parchis

SRC=src/main.c \
    src/colores.c \
    src/memoria_compartida.c \
    src/sincronizacion.c \
    src/estadisticas.c \
    src/tablero.c \
    src/ficha.c \
    src/jugador.c \
    src/ipc_pipes.c \
    src/ipc_colas.c \
    src/ipc_sockets.c \
    src/arbitro.c

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)