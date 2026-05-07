#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "ficha.h"
#include "tablero.h"
#include "colores.h"
#include "config.h"

void crear_hilos_fichas(int id_jugador, EstadoJuego *estado)
{
    pthread_t hilos[NUM_FICHAS];
    FichaContexto contextos[NUM_FICHAS];

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

    printf("Jugador %s: ficha %d creada como hilo\n",
           nombre_jugador(ctx->id_jugador),
           ctx->id_ficha);

    return NULL;
}

int mover_ficha(FichaContexto *ctx, int dado)
{
    if (ctx == NULL) {
        return FALSO;
    }

    return mover_en_tablero(
        ctx->estado,
        ctx->id_jugador,
        ctx->id_ficha,
        dado
    );
}
