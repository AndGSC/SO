#ifndef TABLERO_H
#define TABLERO_H

#include "tipos.h"

void inicializar_tablero(EstadoJuego *estado);
void imprimir_tablero(EstadoJuego *estado);
int calcular_destino(EstadoJuego *estado, int jugador, int ficha, int dado);
int mover_en_tablero(EstadoJuego *estado, int jugador, int ficha, int dado);
int ficha_puede_mover(EstadoJuego *estado, int jugador, int ficha, int dado);
int casilla_es_pasillo(int posicion);
int casilla_es_meta(int posicion);
void enviar_ficha_a_base(EstadoJuego *estado, int jugador, int ficha);
int jugador_gano(EstadoJuego *estado, int jugador);

#endif
