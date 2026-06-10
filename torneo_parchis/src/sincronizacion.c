#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "sincronizacion.h"
#include "config.h"

void inicializar_sincronizacion(EstadoJuego *estado)
{
    pthread_mutexattr_t attr;

    if (pthread_mutexattr_init(&attr) != 0) {
        perror("Error al inicializar atributos del mutex");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED) != 0) {
        perror("Error al configurar mutex compartido entre procesos");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < TAM_TABLERO; i++) {
        if (pthread_mutex_init(&estado->mutex_casilla[i], &attr) != 0) {
            perror("Error al inicializar mutex de casilla");
            exit(EXIT_FAILURE);
        }
    }

    if (pthread_mutex_init(&estado->mutex_estadisticas, &attr) != 0) {
        perror("Error al inicializar mutex de estadisticas");
        exit(EXIT_FAILURE);
    }

    if (pthread_mutex_init(&estado->mutex_juego, &attr) != 0) {
        perror("Error al inicializar mutex de estado del juego");
        exit(EXIT_FAILURE);
    }

    pthread_mutexattr_destroy(&attr);

    if (sem_init(&estado->sem_meta, 1, CAPACIDAD_META) == -1) {
        perror("Error al inicializar semaforo de meta");
        exit(EXIT_FAILURE);
    }

    if (sem_init(&estado->sem_pasillo, 1, CAPACIDAD_PASILLO) == -1) {
        perror("Error al inicializar semaforo de pasillo");
        exit(EXIT_FAILURE);
    }
}

void destruir_sincronizacion(EstadoJuego *estado)
{
    for (int i = 0; i < TAM_TABLERO; i++) {
        pthread_mutex_destroy(&estado->mutex_casilla[i]);
    }

    pthread_mutex_destroy(&estado->mutex_estadisticas);
    pthread_mutex_destroy(&estado->mutex_juego);

    sem_destroy(&estado->sem_meta);
    sem_destroy(&estado->sem_pasillo);
}

void bloquear_casilla(EstadoJuego *estado, int posicion)
{
    if (posicion < 0 || posicion >= TAM_TABLERO) {
        return;
    }

    pthread_mutex_lock(&estado->mutex_casilla[posicion]);
}

void desbloquear_casilla(EstadoJuego *estado, int posicion)
{
    if (posicion < 0 || posicion >= TAM_TABLERO) {
        return;
    }

    pthread_mutex_unlock(&estado->mutex_casilla[posicion]);
}

void bloquear_casillas_movimiento(EstadoJuego *estado, int origen, int destino)
{
    if (origen == destino) {
        bloquear_casilla(estado, origen);
        return;
    }

    if (origen < 0) {
        bloquear_casilla(estado, destino);
        return;
    }

    if (destino < 0) {
        bloquear_casilla(estado, origen);
        return;
    }

    if (origen < destino) {
        bloquear_casilla(estado, origen);
        bloquear_casilla(estado, destino);
    } else {
        bloquear_casilla(estado, destino);
        bloquear_casilla(estado, origen);
    }
}

void desbloquear_casillas_movimiento(EstadoJuego *estado, int origen, int destino)
{
    if (origen == destino) {
        desbloquear_casilla(estado, origen);
        return;
    }

    if (origen < 0) {
        desbloquear_casilla(estado, destino);
        return;
    }

    if (destino < 0) {
        desbloquear_casilla(estado, origen);
        return;
    }

    if (origen < destino) {
        desbloquear_casilla(estado, destino);
        desbloquear_casilla(estado, origen);
    } else {
        desbloquear_casilla(estado, origen);
        desbloquear_casilla(estado, destino);
    }
}

void bloquear_estadisticas(EstadoJuego *estado)
{
    pthread_mutex_lock(&estado->mutex_estadisticas);
}

void desbloquear_estadisticas(EstadoJuego *estado)
{
    pthread_mutex_unlock(&estado->mutex_estadisticas);
}

void bloquear_juego(EstadoJuego *estado)
{
    pthread_mutex_lock(&estado->mutex_juego);
}

void desbloquear_juego(EstadoJuego *estado)
{
    pthread_mutex_unlock(&estado->mutex_juego);
}

void entrar_zona_meta(EstadoJuego *estado)
{
    if (sem_wait(&estado->sem_meta) == -1) {
        perror("Error en sem_wait de meta");
        exit(EXIT_FAILURE);
    }
}

void salir_zona_meta(EstadoJuego *estado)
{
    if (sem_post(&estado->sem_meta) == -1) {
        perror("Error en sem_post de meta");
        exit(EXIT_FAILURE);
    }
}

void entrar_pasillo(EstadoJuego *estado)
{
    if (sem_wait(&estado->sem_pasillo) == -1) {
        perror("Error en sem_wait de pasillo");
        exit(EXIT_FAILURE);
    }
}

void salir_pasillo(EstadoJuego *estado)
{
    if (sem_post(&estado->sem_pasillo) == -1) {
        perror("Error en sem_post de pasillo");
        exit(EXIT_FAILURE);
    }
}
