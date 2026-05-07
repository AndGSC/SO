#include <stdio.h>

#include "estadisticas.h"
#include "sincronizacion.h"
#include "colores.h"
#include "config.h"

void inicializar_estadisticas(EstadoJuego *estado)
{
    bloquear_estadisticas(estado);

    for (int i = 0; i < NUM_JUGADORES; i++) {
        estado->estadisticas[i].movimientos = 0;
        estado->estadisticas[i].capturas = 0;
        estado->estadisticas[i].bloqueos = 0;
        estado->estadisticas[i].fichas_meta = 0;
        estado->estadisticas[i].turnos_jugados = 0;
    }

    desbloquear_estadisticas(estado);
}

void registrar_movimiento(EstadoJuego *estado, int jugador)
{
    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    bloquear_estadisticas(estado);
    estado->estadisticas[jugador].movimientos++;
    desbloquear_estadisticas(estado);
}

void registrar_captura(EstadoJuego *estado, int jugador)
{
    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    bloquear_estadisticas(estado);
    estado->estadisticas[jugador].capturas++;
    desbloquear_estadisticas(estado);
}

void registrar_bloqueo(EstadoJuego *estado, int jugador)
{
    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    bloquear_estadisticas(estado);
    estado->estadisticas[jugador].bloqueos++;
    desbloquear_estadisticas(estado);
}

void registrar_meta(EstadoJuego *estado, int jugador)
{
    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    bloquear_estadisticas(estado);
    estado->estadisticas[jugador].fichas_meta++;
    desbloquear_estadisticas(estado);
}

void registrar_turno(EstadoJuego *estado, int jugador)
{
    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    bloquear_estadisticas(estado);
    estado->estadisticas[jugador].turnos_jugados++;
    desbloquear_estadisticas(estado);
}

void imprimir_estadisticas_finales(EstadoJuego *estado)
{
    bloquear_estadisticas(estado);

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

    desbloquear_estadisticas(estado);
}
