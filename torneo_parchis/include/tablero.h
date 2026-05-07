#ifndef TABLERO_H
#define TABLERO_H

#include "tipos.h"
void inicializar_tablero(EstadoJuego *estado);

/*
 * Muestra el estado actual del tablero en consola.
 */
void imprimir_tablero(EstadoJuego *estado);

/*
 * Calcula la casilla destino de una ficha según el dado.
 */
int calcular_destino(EstadoJuego *estado, int jugador, int ficha, int dado);

int mover_en_tablero(EstadoJuego *estado, int jugador, int ficha, int dado);

/*
 * Indica si una posición pertenece a un pasillo estrecho.
 */
int casilla_es_pasillo(int posicion);

/*
 * Indica si una posición representa entrada o zona de meta.
 */
int casilla_es_meta(int posicion);

/*
 * Envía una ficha de regreso a la base.
 */
void enviar_ficha_a_base(EstadoJuego *estado, int jugador, int ficha);

int jugador_gano(EstadoJuego *estado, int jugador);

#endif
