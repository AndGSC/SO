#include <stdio.h>

#include "arbitro.h"
#include "ipc_sockets.h"
#include "ipc_colas.h"
#include "tablero.h"
#include "sincronizacion.h"
#include "estadisticas.h"
#include "colores.h"
#include "config.h"

void ejecutar_arbitro(EstadoJuego *estado)
{
    if (estado == NULL) {
        return;
    }

    printf("\nArbitro iniciado. Controlando el torneo...\n");

    imprimir_tablero(estado);

    ejecutar_round_robin(estado);

    if (estado->ganador != SIN_GANADOR) {
        printf("\nGanador del torneo: %s\n", nombre_jugador(estado->ganador));
    } else {
        printf("\nEl torneo finalizo sin ganador.\n");
    }
}

void ejecutar_round_robin(EstadoJuego *estado)
{
    int jugador_actual;
    int turno;

    if (estado == NULL) {
        return;
    }

    turno = 0;

    while (estado->juego_terminado == JUEGO_ACTIVO && turno < MAX_TURNOS) {
        jugador_actual = turno % NUM_JUGADORES;

        estado->turno_actual = turno;

        printf("\n========== TURNO %d ==========\n", turno + 1);
        printf("Turno asignado a: %s\n", nombre_jugador(jugador_actual));

        enviar_turno_jugador(jugador_actual);
        recibir_respuesta_jugador(jugador_actual);

        recibir_eventos_pendientes();

        imprimir_tablero(estado);

        jugador_actual = verificar_ganador(estado);

        if (jugador_actual != SIN_GANADOR) {
            finalizar_juego(estado, jugador_actual);
            break;
        }

        turno++;
    }

    if (turno >= MAX_TURNOS && estado->juego_terminado != JUEGO_TERMINADO) {
        bloquear_juego(estado);
        estado->juego_terminado = JUEGO_TERMINADO;
        estado->ganador = SIN_GANADOR;
        desbloquear_juego(estado);
    }
}

void enviar_turno_jugador(int id_jugador)
{
    if (id_jugador < 0 || id_jugador >= NUM_JUGADORES) {
        return;
    }

    enviar_turno_socket(id_jugador);
}

void recibir_respuesta_jugador(int id_jugador)
{
    if (id_jugador < 0 || id_jugador >= NUM_JUGADORES) {
        return;
    }

    recibir_respuesta_socket(id_jugador);
}

int verificar_ganador(EstadoJuego *estado)
{
    if (estado == NULL) {
        return SIN_GANADOR;
    }

    for (int jugador = 0; jugador < NUM_JUGADORES; jugador++) {
        if (jugador_gano(estado, jugador) == VERDADERO) {
            return jugador;
        }
    }

    return SIN_GANADOR;
}

void finalizar_juego(EstadoJuego *estado, int ganador)
{
    if (estado == NULL) {
        return;
    }

    bloquear_juego(estado);
    estado->juego_terminado = JUEGO_TERMINADO;
    estado->ganador = ganador;
    desbloquear_juego(estado);

    if (ganador != SIN_GANADOR) {
        printf("\nEl jugador %s ha ganado el torneo.\n",
               nombre_jugador(ganador));
    }
}
