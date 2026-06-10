#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "ficha.h"
#include "tablero.h"
#include "colores.h"
#include "config.h"

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond_orden;
    pthread_cond_t cond_listo;

    int turno_id;
    int dado_actual;
    int movimiento_reclamado;
    int resultado;
    int ficha_movida;
    int fichas_respondieron;
    int terminar;
} ControlFichas;

static ControlFichas control;
static pthread_t hilos[NUM_FICHAS];
static FichaContexto contextos[NUM_FICHAS];

static int ficha_valida(int id_ficha)
{
    return id_ficha >= 0 && id_ficha < NUM_FICHAS;
}

static int jugador_valido(int id_jugador)
{
    return id_jugador >= 0 && id_jugador < NUM_JUGADORES;
}

void crear_hilos_fichas(int id_jugador, EstadoJuego *estado)
{
    if (!jugador_valido(id_jugador) || estado == NULL) {
        return;
    }

    if (pthread_mutex_init(&control.mutex, NULL) != 0 ||
        pthread_cond_init(&control.cond_orden, NULL) != 0 ||
        pthread_cond_init(&control.cond_listo, NULL) != 0) {
        perror("Error al inicializar control de hilos ficha");
        exit(EXIT_FAILURE);
    }

    control.turno_id = 0;
    control.dado_actual = 0;
    control.movimiento_reclamado = FALSO;
    control.resultado = FALSO;
    control.ficha_movida = -1;
    control.fichas_respondieron = 0;
    control.terminar = FALSO;

    for (int i = 0; i < NUM_FICHAS; i++) {
        contextos[i].id_jugador = id_jugador;
        contextos[i].id_ficha = i;
        contextos[i].estado = estado;

        if (pthread_create(&hilos[i], NULL, ejecutar_ficha, &contextos[i]) != 0) {
            perror("Error al crear hilo de ficha");
            exit(EXIT_FAILURE);
        }
    }
}

void *ejecutar_ficha(void *arg)
{
    FichaContexto *ctx = (FichaContexto *) arg;
    int ultimo_turno = 0;
    int dado;
    int resultado;

    if (ctx == NULL ||
        !jugador_valido(ctx->id_jugador) ||
        !ficha_valida(ctx->id_ficha)) {
        pthread_exit(NULL);
    }

    printf("Jugador %s: ficha %d creada como hilo\n",
           nombre_jugador(ctx->id_jugador),
           ctx->id_ficha);

    pthread_mutex_lock(&control.mutex);

    while (1) {
        while (control.turno_id == ultimo_turno &&
               control.terminar == FALSO) {
            pthread_cond_wait(&control.cond_orden, &control.mutex);
        }

        if (control.terminar == VERDADERO) {
            break;
        }

        ultimo_turno = control.turno_id;
        dado = control.dado_actual;

        if (control.movimiento_reclamado == FALSO &&
            ficha_puede_mover(ctx->estado,
                              ctx->id_jugador,
                              ctx->id_ficha,
                              dado) == VERDADERO) {

            control.movimiento_reclamado = VERDADERO;
            pthread_mutex_unlock(&control.mutex);

            resultado = mover_ficha(ctx, dado);

            pthread_mutex_lock(&control.mutex);

            if (resultado == VERDADERO) {
                control.resultado = VERDADERO;
                control.ficha_movida = ctx->id_ficha;
            }
        }

        control.fichas_respondieron++;

        if (control.fichas_respondieron == NUM_FICHAS) {
            pthread_cond_signal(&control.cond_listo);
        }
    }

    pthread_mutex_unlock(&control.mutex);

    pthread_exit(NULL);
}

int ejecutar_turno_fichas(int dado, int *ficha_movida)
{
    int resultado;

    if (dado < DADO_MIN || dado > DADO_MAX) {
        return FALSO;
    }

    pthread_mutex_lock(&control.mutex);

    control.dado_actual = dado;
    control.movimiento_reclamado = FALSO;
    control.resultado = FALSO;
    control.ficha_movida = -1;
    control.fichas_respondieron = 0;
    control.turno_id++;

    pthread_cond_broadcast(&control.cond_orden);

    while (control.fichas_respondieron < NUM_FICHAS) {
        pthread_cond_wait(&control.cond_listo, &control.mutex);
    }

    resultado = control.resultado;

    if (ficha_movida != NULL) {
        *ficha_movida = control.ficha_movida;
    }

    pthread_mutex_unlock(&control.mutex);

    return resultado;
}

void finalizar_hilos_fichas(void)
{
    pthread_mutex_lock(&control.mutex);
    control.terminar = VERDADERO;
    pthread_cond_broadcast(&control.cond_orden);
    pthread_mutex_unlock(&control.mutex);

    for (int i = 0; i < NUM_FICHAS; i++) {
        if (pthread_join(hilos[i], NULL) != 0) {
            perror("Error al esperar hilo de ficha");
            exit(EXIT_FAILURE);
        }
    }

    pthread_mutex_destroy(&control.mutex);
    pthread_cond_destroy(&control.cond_orden);
    pthread_cond_destroy(&control.cond_listo);
}

int mover_ficha(FichaContexto *ctx, int dado)
{
    if (ctx == NULL || ctx->estado == NULL) {
        return FALSO;
    }

    if (!jugador_valido(ctx->id_jugador) || !ficha_valida(ctx->id_ficha)) {
        return FALSO;
    }

    if (dado < DADO_MIN || dado > DADO_MAX) {
        return FALSO;
    }

    return mover_en_tablero(
        ctx->estado,
        ctx->id_jugador,
        ctx->id_ficha,
        dado
    );
}
