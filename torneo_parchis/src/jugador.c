#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "jugador.h"
#include "ficha.h"
#include "tablero.h"
#include "estadisticas.h"
#include "ipc_sockets.h"
#include "colores.h"
#include "config.h"

static int jugador_valido(int id_jugador)
{
    return id_jugador >= 0 && id_jugador < NUM_JUGADORES;
}

void ejecutar_jugador(int id_jugador, EstadoJuego *estado)
{
    if (!jugador_valido(id_jugador) || estado == NULL) {
        return;
    }

    srand(time(NULL) ^ getpid());

    printf("Proceso jugador %s iniciado. PID: %d\n",
           nombre_jugador(id_jugador),
           getpid());

    crear_hilos_fichas(id_jugador, estado);

    while (estado->juego_terminado == JUEGO_ACTIVO) {
        esperar_turno_socket(id_jugador);

        if (estado->juego_terminado == JUEGO_TERMINADO) {
            break;
        }

        procesar_turno_jugador(id_jugador, estado);

        enviar_respuesta_socket(id_jugador, "Turno completado");
    }

    finalizar_hilos_fichas();

    printf("Proceso jugador %s finalizado.\n",
           nombre_jugador(id_jugador));
}

/*
 * El hilo principal del proceso jugador lanza el dado y delega el
 * movimiento a los hilos ficha: los 4 hilos compiten por reclamar
 * el movimiento del turno sobre el tablero compartido.
 */
void procesar_turno_jugador(int id_jugador, EstadoJuego *estado)
{
    int dado;
    int ficha;

    if (!jugador_valido(id_jugador) || estado == NULL) {
        return;
    }

    if (estado->juego_terminado == JUEGO_TERMINADO) {
        return;
    }

    dado = lanzar_dado();

    printf("Jugador %s lanza dado: %d\n",
           nombre_jugador(id_jugador),
           dado);

    ficha = -1;

    if (ejecutar_turno_fichas(dado, &ficha) == VERDADERO) {
        printf("Jugador %s movio la ficha %d.\n",
               nombre_jugador(id_jugador),
               ficha);
    } else {
        printf("Jugador %s no pudo mover ninguna ficha.\n",
               nombre_jugador(id_jugador));

        registrar_bloqueo(estado, id_jugador);
    }

    registrar_turno(estado, id_jugador);
}

int lanzar_dado(void)
{
    return (rand() % DADO_MAX) + DADO_MIN;
}
