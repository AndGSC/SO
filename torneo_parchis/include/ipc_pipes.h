#ifndef IPC_PIPES_H
#define IPC_PIPES_H

#include "tipos.h"

void crear_pipes(void);
void cerrar_pipes_no_usados(int id_jugador);
void enviar_estadisticas_pipe(int id_jugador, EstadisticasJugador estadisticas);
void recibir_estadisticas_pipe(EstadoJuego *estado);

#endif
