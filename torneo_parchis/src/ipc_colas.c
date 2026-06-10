#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>

#include "ipc_colas.h"
#include "colores.h"
#include "config.h"

static int id_cola_mensajes = -1;

/*
 * La cola se crea antes del fork(), por lo que
 * todos los procesos hijos heredan el identificador.
 */
void crear_cola_mensajes(void)
{
    id_cola_mensajes = msgget(IPC_PRIVATE, IPC_CREAT | 0600);

    if (id_cola_mensajes == -1) {
        perror("Error al crear cola de mensajes");
        exit(EXIT_FAILURE);
    }
}

void enviar_evento(int tipo_evento, int jugador, int ficha, const char *descripcion)
{
    MensajeEvento mensaje;

    if (id_cola_mensajes == -1) {
        return;
    }

    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    mensaje.tipo_mensaje = 1;
    mensaje.tipo_evento = tipo_evento;
    mensaje.jugador = jugador;
    mensaje.ficha = ficha;

    if (descripcion != NULL) {
        strncpy(mensaje.descripcion, descripcion, TAM_DESCRIPCION_EVENTO - 1);
        mensaje.descripcion[TAM_DESCRIPCION_EVENTO - 1] = '\0';
    } else {
        mensaje.descripcion[0] = '\0';
    }

    if (msgsnd(id_cola_mensajes,
               &mensaje,
               sizeof(MensajeEvento) - sizeof(long),
               IPC_NOWAIT) == -1) {
        perror("Error al enviar evento a la cola de mensajes");
    }
}

void recibir_eventos_pendientes(void)
{
    MensajeEvento mensaje;
    ssize_t recibido;

    if (id_cola_mensajes == -1) {
        return;
    }

    while (1) {
        recibido = msgrcv(id_cola_mensajes,
                          &mensaje,
                          sizeof(MensajeEvento) - sizeof(long),
                          0,
                          IPC_NOWAIT);

        if (recibido == -1) {
            if (errno == ENOMSG) {
                break;
            }

            perror("Error al recibir evento de la cola de mensajes");
            break;
        }

        printf("[EVENTO] Jugador %s",
               nombre_jugador(mensaje.jugador));

        if (mensaje.ficha >= 0) {
            printf(" - Ficha %d", mensaje.ficha);
        }

        printf(": %s\n", mensaje.descripcion);
    }
}

void eliminar_cola_mensajes(void)
{
    if (id_cola_mensajes == -1) {
        return;
    }

    if (msgctl(id_cola_mensajes, IPC_RMID, NULL) == -1) {
        perror("Error al eliminar cola de mensajes");
        exit(EXIT_FAILURE);
    }

    id_cola_mensajes = -1;
}
