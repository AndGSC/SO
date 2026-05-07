#ifndef TIPOS_H
#define TIPOS_H

#include <pthread.h>
#include <semaphore.h>

#include "config.h"
#include "colores.h"

typedef struct {
    int ocupada;
    int jugador;
    int ficha;
} Casilla;

typedef struct {
    int movimientos;
    int capturas;
    int bloqueos;
    int fichas_meta;
    int turnos_jugados;
} EstadisticasJugador;

typedef struct {
    Casilla casillas[TAM_TABLERO];

    int posiciones[NUM_JUGADORES][NUM_FICHAS];
    int fichas_en_meta[NUM_JUGADORES];

    EstadisticasJugador estadisticas[NUM_JUGADORES];

    pthread_mutex_t mutex_casilla[TAM_TABLERO];

    /*
     * Protege modificaciones concurrentes sobre las estadísticas.
     */
    pthread_mutex_t mutex_estadisticas;

    sem_t sem_meta;
    sem_t sem_pasillo;

    int juego_terminado;
    int ganador;
    int turno_actual;

} EstadoJuego;

typedef struct {
    int id_jugador;
    EstadoJuego *estado;
} JugadorContexto;

typedef struct {
    int id_jugador;
    int id_ficha;
    EstadoJuego *estado;
} FichaContexto;

#endif
