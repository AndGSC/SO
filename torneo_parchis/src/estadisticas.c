#include <stdio.h>

#include "estadisticas.h"
#include "colores.h"
#include "config.h"

void inicializar_estadisticas(EstadoJuego *estado)
{
    for (int i = 0; i < NUM_JUGADORES; i++) {
        estado->estadisticas[i].movimientos = 0;
        estado->estadisticas[i].capturas = 0;
        estado->estadisticas[i].bloqueos = 0;
        estado->estadisticas[i].fichas_meta = 0;
        estado->estadisticas[i].turnos_jugados = 0;
    }
}

void registrar_movimiento(EstadoJuego *estado, int jugador)
{
    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    estado->estadisticas[jugador].movimientos++;
}

void registrar_captura(EstadoJuego *estado, int jugador)
{
    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    estado->estadisticas[jugador].capturas++;
}

void registrar_bloqueo(EstadoJuego *estado, int jugador)
{
    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    estado->estadisticas[jugador].bloqueos++;
}

void registrar_meta(EstadoJuego *estado, int jugador)
{
    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    estado->estadisticas[jugador].fichas_meta++;
}

void registrar_turno(EstadoJuego *estado, int jugador)
{
    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    estado->estadisticas[jugador].turnos_jugados++;
}

void imprimir_estadisticas_finales(EstadoJuego *estado)
{
    printf("\n========== ESTADISTICAS FINALES ==========\n");

    for (int i = 0; i < NUM_JUGADORES; i++) {
        printf("\nJugador: %s\n", nombre_jugador(i));
        printf("  Turnos jugados : %d\n", estado->estadisticas[i].turnos_jugados);
        printf("  Movimientos    : %d\n", estado->estadisticas[i].movimientos);
        printf("  Capturas       : %d\n", estado->estadisticas[i].capturas);
        printf("  Bloqueos       : %d\n", estado->estadisticas[i].bloqueos);
        printf("  Fichas en meta : %d\n", estado->estadisticas[i].fichas_meta);
    }

    printf("\n==========================================\n");
}
