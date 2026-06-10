#ifndef SINCRONIZACION_H
#define SINCRONIZACION_H

#include "tipos.h"

void inicializar_sincronizacion(EstadoJuego *estado);
void destruir_sincronizacion(EstadoJuego *estado);

void bloquear_casilla(EstadoJuego *estado, int posicion);
void desbloquear_casilla(EstadoJuego *estado, int posicion);

void bloquear_casillas_movimiento(EstadoJuego *estado, int origen, int destino);
void desbloquear_casillas_movimiento(EstadoJuego *estado, int origen, int destino);

void bloquear_estadisticas(EstadoJuego *estado);
void desbloquear_estadisticas(EstadoJuego *estado);

void bloquear_juego(EstadoJuego *estado);
void desbloquear_juego(EstadoJuego *estado);

void entrar_zona_meta(EstadoJuego *estado);
void salir_zona_meta(EstadoJuego *estado);

void entrar_pasillo(EstadoJuego *estado);
void salir_pasillo(EstadoJuego *estado);

#endif
