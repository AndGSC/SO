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

/*
 * Selecciona una ficha válida para mover.
 * Retorna el índice de la ficha seleccionada o -1 si no hay ficha disponible.
 */
int seleccionar_ficha(int id_jugador, EstadoJuego *estado, int dado);

#endif
