#ifndef SINCRONIZACION_H
#define SINCRONIZACION_H

#include "tipos.h"

/*
 * Inicializa mutex por casilla y semáforos binarios.
 */
void inicializar_sincronizacion(EstadoJuego *estado);

/*
 * Destruye mutex y semáforos.
 */
void destruir_sincronizacion(EstadoJuego *estado);

/*
 * Bloqueo individual de casillas.
 */
void bloquear_casilla(EstadoJuego *estado, int posicion);
void desbloquear_casilla(EstadoJuego *estado, int posicion);

/*
 * Bloqueo seguro de origen y destino para evitar deadlock.
 */
void bloquear_casillas_movimiento(EstadoJuego *estado, int origen, int destino);
void desbloquear_casillas_movimiento(EstadoJuego *estado, int origen, int destino);

/*
 * Semáforo binario para zona de meta.
 */
void entrar_zona_meta(EstadoJuego *estado);
void salir_zona_meta(EstadoJuego *estado);

/*
 * Semáforo binario para pasillo estrecho.
 */
void entrar_pasillo(EstadoJuego *estado);
void salir_pasillo(EstadoJuego *estado);

#endif
