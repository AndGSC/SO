#ifndef JUGADOR_H
#define JUGADOR_H

#include "tipos.h"

/*
 * Ejecuta la lógica principal de un proceso jugador.
 */
void ejecutar_jugador(int id_jugador, EstadoJuego *estado);

/*
 * Procesa un turno asignado por el árbitro.
 */
void procesar_turno_jugador(int id_jugador, EstadoJuego *estado);

/*
 * Genera un número aleatorio entre DADO_MIN y DADO_MAX.
 */
int lanzar_dado(void);

#endif
