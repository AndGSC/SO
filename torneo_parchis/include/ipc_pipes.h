#ifndef IPC_PIPES_H
#define IPC_PIPES_H

#include "tipos.h"

/*
 * Crea los pipes para comunicación entre padre e hijos.
 */
void crear_pipes(void);

/*
 * Cierra extremos de pipes que un jugador no necesita usar.
 */
void cerrar_pipes_no_usados(int id_jugador);

/*
 * Envía las estadísticas de un jugador al proceso padre.
 */
void enviar_estadisticas_pipe(int id_jugador, EstadisticasJugador estadisticas);

/*
 * Recibe las estadísticas enviadas por los procesos hijos.
 */
void recibir_estadisticas_pipe(EstadoJuego *estado);

#endif
