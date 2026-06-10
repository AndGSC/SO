#include <stdio.h>

#include "tablero.h"
#include "sincronizacion.h"
#include "estadisticas.h"
#include "ipc_colas.h"
#include "colores.h"
#include "config.h"

void inicializar_tablero(EstadoJuego *estado)
{
    for (int i = 0; i < TAM_TABLERO; i++) {
        estado->casillas[i].ocupada = FALSO;
        estado->casillas[i].jugador = SIN_GANADOR;
        estado->casillas[i].ficha = -1;
    }

    for (int jugador = 0; jugador < NUM_JUGADORES; jugador++) {
        estado->fichas_en_meta[jugador] = 0;

        for (int ficha = 0; ficha < NUM_FICHAS; ficha++) {
            estado->posiciones[jugador][ficha] = BASE;
        }
    }

    estado->juego_terminado = JUEGO_ACTIVO;
    estado->ganador = SIN_GANADOR;
    estado->turno_actual = 0;
}


void imprimir_tablero(EstadoJuego *estado)
{
    int en_base;
    int en_tablero;

    printf("\n+======================= TABLERO =======================+\n");

    for (int i = 0; i < TAM_TABLERO; i++) {
        char marca = casilla_es_pasillo(i) ? '*' : ' ';

        if (estado->casillas[i].ocupada == VERDADERO) {
            printf("[%02d%c%s%c%d%s] ",
                   i,
                   marca,
                   color_jugador(estado->casillas[i].jugador),
                   simbolo_jugador(estado->casillas[i].jugador),
                   estado->casillas[i].ficha,
                   COLOR_RESET);
        } else {
            printf("[%02d%c..] ", i, marca);
        }

        if ((i + 1) % 5 == 0) {
            printf("\n");
        }
    }

    printf("(* = pasillo estrecho controlado por semaforo)\n\n");

    printf("%-10s %6s %9s %6s\n", "Jugador", "Base", "Tablero", "Meta");

    for (int jugador = 0; jugador < NUM_JUGADORES; jugador++) {
        en_base = 0;
        en_tablero = 0;

        for (int ficha = 0; ficha < NUM_FICHAS; ficha++) {
            if (estado->posiciones[jugador][ficha] == BASE) {
                en_base++;
            } else if (estado->posiciones[jugador][ficha] != META) {
                en_tablero++;
            }
        }

        printf("%s%-10s%s %6d %9d %3d/%d\n",
               color_jugador(jugador),
               nombre_jugador(jugador),
               COLOR_RESET,
               en_base,
               en_tablero,
               estado->fichas_en_meta[jugador],
               NUM_FICHAS);
    }

    printf("+=======================================================+\n");
}

int calcular_destino(EstadoJuego *estado, int jugador, int ficha, int dado)
{
    int posicion_actual;

    if (estado == NULL) {
        return BASE;
    }

    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return BASE;
    }

    if (ficha < 0 || ficha >= NUM_FICHAS) {
        return BASE;
    }

    posicion_actual = estado->posiciones[jugador][ficha];

    if (posicion_actual == META) {
        return META;
    }

    if (posicion_actual == BASE) {
        return dado - 1;
    }

    if (posicion_actual + dado >= TAM_TABLERO) {
        return META;
    }

    return posicion_actual + dado;
}

/*
 * Verifica si una ficha tiene un movimiento posible con el dado dado.
 * La lectura de la casilla destino se hace con su mutex tomado para
 * no leer un estado intermedio de otro movimiento en curso.
 */
int ficha_puede_mover(EstadoJuego *estado, int jugador, int ficha, int dado)
{
    int posicion;
    int destino;
    int puede;

    if (estado == NULL) {
        return FALSO;
    }

    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return FALSO;
    }

    if (ficha < 0 || ficha >= NUM_FICHAS) {
        return FALSO;
    }

    if (dado < DADO_MIN || dado > DADO_MAX) {
        return FALSO;
    }

    posicion = estado->posiciones[jugador][ficha];

    if (posicion == META) {
        return FALSO;
    }

    destino = calcular_destino(estado, jugador, ficha, dado);

    if (destino == META) {
        return VERDADERO;
    }

    if (destino < 0 || destino >= TAM_TABLERO) {
        return FALSO;
    }

    bloquear_casilla(estado, destino);

    puede = (estado->casillas[destino].ocupada == FALSO ||
             estado->casillas[destino].jugador != jugador);

    desbloquear_casilla(estado, destino);

    return puede;
}

int casilla_es_pasillo(int posicion)
{
    return posicion == 10 || posicion == 11 ||
           posicion == 20 || posicion == 21 ||
           posicion == 30 || posicion == 31;
}

int casilla_es_meta(int posicion)
{
    return posicion == META;
}

void enviar_ficha_a_base(EstadoJuego *estado, int jugador, int ficha)
{
    if (estado == NULL) {
        return;
    }

    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return;
    }

    if (ficha < 0 || ficha >= NUM_FICHAS) {
        return;
    }

    estado->posiciones[jugador][ficha] = BASE;
}

int mover_en_tablero(EstadoJuego *estado, int jugador, int ficha, int dado)
{
    int origen;
    int destino;
    int jugador_rival;
    int ficha_rival;
    char descripcion[TAM_DESCRIPCION_EVENTO];

    if (estado == NULL) {
        return FALSO;
    }

    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return FALSO;
    }

    if (ficha < 0 || ficha >= NUM_FICHAS) {
        return FALSO;
    }

    if (dado < DADO_MIN || dado > DADO_MAX) {
        return FALSO;
    }

    origen = estado->posiciones[jugador][ficha];

    if (origen == META) {
        return FALSO;
    }

    destino = calcular_destino(estado, jugador, ficha, dado);

    if (destino == META) {
        entrar_zona_meta(estado);

        if (origen >= 0 && origen < TAM_TABLERO) {
            bloquear_casilla(estado, origen);

            estado->casillas[origen].ocupada = FALSO;
            estado->casillas[origen].jugador = SIN_GANADOR;
            estado->casillas[origen].ficha = -1;

            desbloquear_casilla(estado, origen);
        }

        estado->posiciones[jugador][ficha] = META;
        estado->fichas_en_meta[jugador]++;

        registrar_movimiento(estado, jugador);
        registrar_meta(estado, jugador);

        snprintf(descripcion,
                 sizeof(descripcion),
                 "La ficha %d de %s llego a meta",
                 ficha,
                 nombre_jugador(jugador));

        enviar_evento(EVENTO_META, jugador, ficha, descripcion);

        salir_zona_meta(estado);

        if (jugador_gano(estado, jugador) == VERDADERO) {
            bloquear_juego(estado);
            estado->juego_terminado = JUEGO_TERMINADO;
            estado->ganador = jugador;
            desbloquear_juego(estado);
        }

        return VERDADERO;
    }

    if (casilla_es_pasillo(destino)) {
        entrar_pasillo(estado);
    }

    bloquear_casillas_movimiento(estado, origen, destino);

    if (estado->casillas[destino].ocupada == VERDADERO) {
        jugador_rival = estado->casillas[destino].jugador;
        ficha_rival = estado->casillas[destino].ficha;

        if (jugador_rival != jugador) {
            enviar_ficha_a_base(estado, jugador_rival, ficha_rival);
            registrar_captura(estado, jugador);

            snprintf(descripcion,
                     sizeof(descripcion),
                     "%s capturo la ficha %d de %s",
                     nombre_jugador(jugador),
                     ficha_rival,
                     nombre_jugador(jugador_rival));

            enviar_evento(EVENTO_CAPTURA, jugador, ficha, descripcion);
        } else {
            desbloquear_casillas_movimiento(estado, origen, destino);

            if (casilla_es_pasillo(destino)) {
                salir_pasillo(estado);
            }

            registrar_bloqueo(estado, jugador);

            snprintf(descripcion,
                     sizeof(descripcion),
                     "La ficha %d de %s no pudo moverse porque la casilla destino esta ocupada por una ficha propia",
                     ficha,
                     nombre_jugador(jugador));

            enviar_evento(EVENTO_BLOQUEO, jugador, ficha, descripcion);

            return FALSO;
        }
    }

    if (origen >= 0 && origen < TAM_TABLERO) {
        estado->casillas[origen].ocupada = FALSO;
        estado->casillas[origen].jugador = SIN_GANADOR;
        estado->casillas[origen].ficha = -1;
    }

    estado->casillas[destino].ocupada = VERDADERO;
    estado->casillas[destino].jugador = jugador;
    estado->casillas[destino].ficha = ficha;

    estado->posiciones[jugador][ficha] = destino;

    registrar_movimiento(estado, jugador);

    snprintf(descripcion,
             sizeof(descripcion),
             "La ficha %d de %s se movio de %d a %d",
             ficha,
             nombre_jugador(jugador),
             origen,
             destino);

    enviar_evento(EVENTO_MOVIMIENTO, jugador, ficha, descripcion);

    desbloquear_casillas_movimiento(estado, origen, destino);

    if (casilla_es_pasillo(destino)) {
        salir_pasillo(estado);
    }

    return VERDADERO;
}

int jugador_gano(EstadoJuego *estado, int jugador)
{
    if (estado == NULL) {
        return FALSO;
    }

    if (jugador < 0 || jugador >= NUM_JUGADORES) {
        return FALSO;
    }

    return estado->fichas_en_meta[jugador] == NUM_FICHAS;
}
