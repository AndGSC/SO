#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "jugador.h"
#include "ficha.h"
#include "tablero.h"
#include "estadisticas.h"
#include "colores.h"
#include "config.h"

static int jugador_valido(int id_jugador)
{
    return id_jugador >= 0 && id_jugador < NUM_JUGADORES;
}

void ejecutar_jugador(int id_jugador, EstadoJuego *estado)
{
    if (!jugador_valido(id_jugador) || estado == NULL) {
        return;
    }

    srand(time(NULL) ^ getpid());

    printf("Proceso jugador %s iniciado. PID: %d\n",
           nombre_jugador(id_jugador),
           getpid());

    crear_hilos_fichas(id_jugador, estado);

    while (estado->juego_terminado == JUEGO_ACTIVO) {
        procesar_turno_jugador(id_jugador, estado);

        /*
         * Temporal:
         * se procesa solo un turno hasta integrar el árbitro,
         * Round Robin y sockets.
         */
        break;
    }

    printf("Proceso jugador %s finalizado.\n", nombre_jugador(id_jugador));
}

void procesar_turno_jugador(int id_jugador, EstadoJuego *estado)
{
    int dado;
    int ficha;
    FichaContexto ctx_ficha;

    if (!jugador_valido(id_jugador) || estado == NULL) {
        return;
    }

    if (estado->juego_terminado == JUEGO_TERMINADO) {
        return;
    }

    dado = lanzar_dado();

    printf("Jugador %s lanza dado: %d\n",
           nombre_jugador(id_jugador),
           dado);

    ficha = seleccionar_ficha(id_jugador, estado, dado);

    if (ficha == -1) {
        printf("Jugador %s no tiene fichas disponibles para mover.\n",
               nombre_jugador(id_jugador));

        registrar_bloqueo(estado, id_jugador);
        registrar_turno(estado, id_jugador);
        return;
    }

    ctx_ficha.id_jugador = id_jugador;
    ctx_ficha.id_ficha = ficha;
    ctx_ficha.estado = estado;

    if (mover_ficha(&ctx_ficha, dado) == VERDADERO) {
        printf("Jugador %s movio la ficha %d.\n",
               nombre_jugador(id_jugador),
               ficha);
    } else {
        printf("Jugador %s no pudo mover la ficha %d.\n",
               nombre_jugador(id_jugador),
               ficha);

        registrar_bloqueo(estado, id_jugador);
    }

    registrar_turno(estado, id_jugador);
}

int lanzar_dado(void)
{
    return (rand() % DADO_MAX) + DADO_MIN;
}

int seleccionar_ficha(int id_jugador, EstadoJuego *estado, int dado)
{
    int posicion;
    int destino;

    if (!jugador_valido(id_jugador) || estado == NULL) {
        return -1;
    }

    if (dado < DADO_MIN || dado > DADO_MAX) {
        return -1;
    }

    for (int ficha = 0; ficha < NUM_FICHAS; ficha++) {
        posicion = estado->posiciones[id_jugador][ficha];

        if (posicion == META) {
            continue;
        }

        destino = calcular_destino(estado, id_jugador, ficha, dado);

        if (destino == META) {
            return ficha;
        }

        if (destino >= 0 && destino < TAM_TABLERO) {
            if (estado->casillas[destino].ocupada == FALSO ||
                estado->casillas[destino].jugador != id_jugador) {
                return ficha;
            }
        }
    }

    return -1;
}
