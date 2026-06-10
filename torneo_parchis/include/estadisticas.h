#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include "tipos.h"

void inicializar_estadisticas(EstadoJuego *estado);

void registrar_movimiento(EstadoJuego *estado, int jugador);
void registrar_captura(EstadoJuego *estado, int jugador);
void registrar_bloqueo(EstadoJuego *estado, int jugador);
void registrar_meta(EstadoJuego *estado, int jugador);
void registrar_turno(EstadoJuego *estado, int jugador);

void imprimir_estadisticas_finales(EstadoJuego *estado);

#endif
