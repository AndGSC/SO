#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "ipc_sockets.h"
#include "colores.h"
#include "config.h"

static int sockets_servidor[NUM_JUGADORES] = {-1, -1, -1, -1};
static int sockets_cliente[NUM_JUGADORES] = {-1, -1, -1, -1};
static int socket_jugador = -1;

static int puerto_jugador(int id_jugador)
{
    return PUERTO_BASE + id_jugador;
}

static int jugador_valido(int id_jugador)
{
    return id_jugador >= 0 && id_jugador < NUM_JUGADORES;
}

static void aceptar_conexion_jugador(int id_jugador)
{
    struct sockaddr_in direccion_cliente;
    socklen_t tam_direccion;

    if (!jugador_valido(id_jugador)) {
        return;
    }

    if (sockets_cliente[id_jugador] != -1) {
        return;
    }

    tam_direccion = sizeof(direccion_cliente);

    sockets_cliente[id_jugador] = accept(
        sockets_servidor[id_jugador],
        (struct sockaddr *) &direccion_cliente,
        &tam_direccion
    );

    if (sockets_cliente[id_jugador] == -1) {
        perror("Error al aceptar conexion del jugador");
        exit(EXIT_FAILURE);
    }
}

void inicializar_sockets_arbitro(void)
{
    struct sockaddr_in direccion;
    int opcion = 1;

    for (int i = 0; i < NUM_JUGADORES; i++) {
        sockets_servidor[i] = socket(AF_INET, SOCK_STREAM, 0);

        if (sockets_servidor[i] == -1) {
            perror("Error al crear socket del arbitro");
            exit(EXIT_FAILURE);
        }

        if (setsockopt(sockets_servidor[i],
                       SOL_SOCKET,
                       SO_REUSEADDR,
                       &opcion,
                       sizeof(opcion)) == -1) {
            perror("Error en setsockopt");
            exit(EXIT_FAILURE);
        }

        memset(&direccion, 0, sizeof(direccion));
        direccion.sin_family = AF_INET;
        direccion.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
        direccion.sin_port = htons(puerto_jugador(i));

        if (bind(sockets_servidor[i],
                 (struct sockaddr *) &direccion,
                 sizeof(direccion)) == -1) {
            perror("Error al hacer bind del socket del arbitro");
            exit(EXIT_FAILURE);
        }

        if (listen(sockets_servidor[i], 1) == -1) {
            perror("Error al poner socket en escucha");
            exit(EXIT_FAILURE);
        }
    }
}

void inicializar_socket_jugador(int id_jugador)
{
    struct sockaddr_in direccion;
    int conectado = FALSO;

    if (!jugador_valido(id_jugador)) {
        return;
    }

    socket_jugador = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_jugador == -1) {
        perror("Error al crear socket del jugador");
        exit(EXIT_FAILURE);
    }

    memset(&direccion, 0, sizeof(direccion));
    direccion.sin_family = AF_INET;
    direccion.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    direccion.sin_port = htons(puerto_jugador(id_jugador));

    for (int intento = 0; intento < 20; intento++) {
        if (connect(socket_jugador,
                    (struct sockaddr *) &direccion,
                    sizeof(direccion)) == 0) {
            conectado = VERDADERO;
            break;
        }

        usleep(100000);
    }

    if (conectado == FALSO) {
        perror("Error al conectar socket del jugador con el arbitro");
        exit(EXIT_FAILURE);
    }

    printf("Jugador %s conectado al arbitro por socket en puerto %d\n",
           nombre_jugador(id_jugador),
           puerto_jugador(id_jugador));
}

void enviar_turno_socket(int id_jugador)
{
    char mensaje[TAM_BUFFER_SOCKET];

    if (!jugador_valido(id_jugador)) {
        return;
    }

    aceptar_conexion_jugador(id_jugador);

    snprintf(mensaje,
             sizeof(mensaje),
             "TURNO %s",
             nombre_jugador(id_jugador));

    if (write(sockets_cliente[id_jugador],
              mensaje,
              strlen(mensaje) + 1) == -1) {
        perror("Error al enviar turno por socket");
        exit(EXIT_FAILURE);
    }
}

void esperar_turno_socket(int id_jugador)
{
    char buffer[TAM_BUFFER_SOCKET];
    ssize_t leidos;

    if (!jugador_valido(id_jugador)) {
        return;
    }

    leidos = read(socket_jugador, buffer, sizeof(buffer) - 1);

    if (leidos == -1) {
        perror("Error al recibir turno por socket");
        exit(EXIT_FAILURE);
    }

    if (leidos == 0) {
        return;
    }

    buffer[leidos] = '\0';

    printf("Jugador %s recibio mensaje: %s\n",
           nombre_jugador(id_jugador),
           buffer);
}

void enviar_respuesta_socket(int id_jugador, const char *mensaje)
{
    char buffer[TAM_BUFFER_SOCKET];

    if (!jugador_valido(id_jugador)) {
        return;
    }

    if (mensaje == NULL) {
        mensaje = "Turno completado";
    }

    snprintf(buffer,
             sizeof(buffer),
             "%s: %s",
             nombre_jugador(id_jugador),
             mensaje);

    if (write(socket_jugador, buffer, strlen(buffer) + 1) == -1) {
        perror("Error al enviar respuesta por socket");
        exit(EXIT_FAILURE);
    }
}

void recibir_respuesta_socket(int id_jugador)
{
    char buffer[TAM_BUFFER_SOCKET];
    ssize_t leidos;

    if (!jugador_valido(id_jugador)) {
        return;
    }

    aceptar_conexion_jugador(id_jugador);

    leidos = read(sockets_cliente[id_jugador],
                  buffer,
                  sizeof(buffer) - 1);

    if (leidos == -1) {
        perror("Error al recibir respuesta por socket");
        exit(EXIT_FAILURE);
    }

    if (leidos == 0) {
        return;
    }

    buffer[leidos] = '\0';

    printf("Respuesta recibida del jugador %s: %s\n",
           nombre_jugador(id_jugador),
           buffer);
}

void cerrar_sockets(void)
{
    if (socket_jugador != -1) {
        close(socket_jugador);
        socket_jugador = -1;
    }

    for (int i = 0; i < NUM_JUGADORES; i++) {
        if (sockets_cliente[i] != -1) {
            close(sockets_cliente[i]);
            sockets_cliente[i] = -1;
        }

        if (sockets_servidor[i] != -1) {
            close(sockets_servidor[i]);
            sockets_servidor[i] = -1;
        }
    }
}
