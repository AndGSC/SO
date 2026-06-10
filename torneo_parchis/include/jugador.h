#ifndef JUGADOR_H
#define JUGADOR_H

#include "tipos.h"

void ejecutar_jugador(int id_jugador, EstadoJuego *estado);
void procesar_turno_jugador(int id_jugador, EstadoJuego *estado);
int lanzar_dado(void);

#endif
