#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "ficha.h"
#include "tablero.h"
#include "colores.h"
#include "config.h"

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
    pthread_t hilos[NUM_FICHAS];
    FichaContexto contextos[NUM_FICHAS];

    if (!jugador_valido(id_jugador) || estado == NULL) {
        return;
    }

    for (int i = 0; i < NUM_FICHAS; i++) {
        contextos[i].id_jugador = id_jugador;
        contextos[i].id_ficha = i;
        contextos[i].estado = estado;

        if (pthread_create(&hilos[i], NULL, ejecutar_ficha, &contextos[i]) != 0) {
            perror("Error al crear hilo de ficha");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_FICHAS; i++) {
        if (pthread_join(hilos[i], NULL) != 0) {
            perror("Error al esperar hilo de ficha");
            exit(EXIT_FAILURE);
        }
    }
}

void *ejecutar_ficha(void *arg)
{
    FichaContexto *ctx = (FichaContexto *) arg;

    if (ctx == NULL) {
        pthread_exit(NULL);
    }

    if (!jugador_valido(ctx->id_jugador) || !ficha_valida(ctx->id_ficha)) {
        pthread_exit(NULL);
    }

    printf("Jugador %s: ficha %d creada como hilo\n",
           nombre_jugador(ctx->id_jugador),
           ctx->id_ficha);

    pthread_exit(NULL);
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
