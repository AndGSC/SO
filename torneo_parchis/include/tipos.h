#ifndef TIPOS_H
#define TIPOS_H

#include <pthread.h>
#include <semaphore.h>

#include "config.h"

/*
 *   FALSO      -> la casilla está libre
 *   VERDADERO  -> la casilla está ocupada
 *
 * jugador:
 *   ROJO, VERDE, AZUL o AMARILLO.
 * ficha:
 *   número de ficha del jugador, de 0 a NUM_FICHAS - 1.
 */
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

/*
 * Estado global del juego.
 *
 * Esta estructura debe ubicarse en memoria compartida para que
 * el proceso padre y los procesos hijos puedan acceder al mismo tablero.
 */
typedef struct {
    Casilla casillas[TAM_TABLERO];

    /*
     * Puede valer:
     *   BASE -> la ficha está en la base
     *   0 a TAM_TABLERO - 1 -> la ficha está en el tablero
     *   META -> la ficha llegó a la meta
     */
    int posiciones[NUM_JUGADORES][NUM_FICHAS];

    /*
     * Cantidad de fichas que han llegado a meta por jugador.
     */
    int fichas_en_meta[NUM_JUGADORES];

    /*
     * Estadísticas de cada jugador.
     */
    EstadisticasJugador estadisticas[NUM_JUGADORES];

    /*
     * Mutex por casilla.
     *
     * Cada casilla tiene su propio mutex para proteger el acceso
     * concurrente durante los movimientos de fichas.
     */
    pthread_mutex_t mutex_casilla[TAM_TABLERO];

    /*
     * Semáforos binarios para zonas especiales.
     *
     * sem_meta:
     *   controla el acceso exclusivo a la zona de meta.
     *
     * sem_pasillo:
     *   controla el acceso exclusivo a los pasillos estrechos.
     */
    sem_t sem_meta;
    sem_t sem_pasillo;

    int juego_terminado;
    int ganador;
    int turno_actual;

} EstadoJuego;

/*
 * Contexto de un proceso jugador.
 *
 * Cada proceso hijo usa esta información para saber
 * qué jugador representa y dónde está el estado compartido.
 */
typedef struct {
    int id_jugador;
    EstadoJuego *estado;
} JugadorContexto;

/*
 * Contexto de una ficha.
 *
 * Cada hilo recibe esta estructura para saber:
 *   - a qué jugador pertenece;
 *   - cuál ficha representa;
 *   - dónde está el estado compartido.
 */
typedef struct {
    int id_jugador;
    int id_ficha;
    EstadoJuego *estado;
} FichaContexto;

#endif
