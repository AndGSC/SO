#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

#include "ipc_pipes.h"
#include "config.h"

/*
 * Un pipe por cada jugador.
 *
 * pipes_estadisticas[i][0] -> extremo de lectura
 * pipes_estadisticas[i][1] -> extremo de escritura
 */
static int pipes_estadisticas[NUM_JUGADORES][2];

void crear_pipes(void)
{
    for (int i = 0; i < NUM_JUGADORES; i++) {
        if (pipe(pipes_estadisticas[i]) == -1) {
            perror("Error al crear pipe");
            exit(EXIT_FAILURE);
        }
    }
}

void cerrar_pipes_no_usados(int id_jugador)
{
    if (id_jugador < 0 || id_jugador >= NUM_JUGADORES) {
        return;
    }

    for (int i = 0; i < NUM_JUGADORES; i++) {
        close(pipes_estadisticas[i][0]);

        if (i != id_jugador) {
            close(pipes_estadisticas[i][1]);
        }
    }
}

void enviar_estadisticas_pipe(int id_jugador, EstadisticasJugador estadisticas)
{
    ssize_t escritos;

    if (id_jugador < 0 || id_jugador >= NUM_JUGADORES) {
        return;
    }

    escritos = write(
        pipes_estadisticas[id_jugador][1],
        &estadisticas,
        sizeof(EstadisticasJugador)
    );

    if (escritos == -1) {
        perror("Error al enviar estadisticas por pipe");
        exit(EXIT_FAILURE);
    }

    close(pipes_estadisticas[id_jugador][1]);
}

void recibir_estadisticas_pipe(EstadoJuego *estado)
{
    EstadisticasJugador recibidas;
    ssize_t leidos;

    for (int i = 0; i < NUM_JUGADORES; i++) {
        close(pipes_estadisticas[i][1]);

        leidos = read(
            pipes_estadisticas[i][0],
            &recibidas,
            sizeof(EstadisticasJugador)
        );

        if (leidos == -1) {
            perror("Error al recibir estadisticas por pipe");
            exit(EXIT_FAILURE);
        }

        if (leidos == sizeof(EstadisticasJugador)) {
            estado->estadisticas[i] = recibidas;
        }

        close(pipes_estadisticas[i][0]);
    }
}
