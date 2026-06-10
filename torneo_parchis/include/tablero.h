#ifndef TABLERO_H
#define TABLERO_H

#include "tipos.h"

/*
 * Inicializa las casillas, posiciones de fichas y estado general del juego.
 */
void inicializar_tablero(EstadoJuego *estado);

/*
 * Muestra el estado actual del tablero en consola.
 */
void imprimir_tablero(EstadoJuego *estado);

/*
 * Calcula la casilla destino de una ficha según el dado.
 */
int calcular_destino(EstadoJuego *estado, int jugador, int ficha, int dado);

/*
 * Mueve una ficha en el tablero compartido.
 * Retorna VERDADERO si el movimiento se realizó, FALSO si no fue posible.
 */
int mover_en_tablero(EstadoJuego *estado, int jugador, int ficha, int dado);

/*
 * Verifica si una ficha tiene un movimiento posible con el dado.
 * Lee la casilla destino con su mutex tomado.
 */
int ficha_puede_mover(EstadoJuego *estado, int jugador, int ficha, int dado);

/*
 * Indica si una posición pertenece a un pasillo estrecho.
 */
int casilla_es_pasillo(int posicion);

/*
 * Indica si una posición representa llegada a meta.
 */
int casilla_es_meta(int posicion);

/*
 * Envía una ficha de regreso a la base.
 */
void enviar_ficha_a_base(EstadoJuego *estado, int jugador, int ficha);

/*
 * Verifica si un jugador ya metió todas sus fichas a meta.
 */
int jugador_gano(EstadoJuego *estado, int jugador);

#endif
