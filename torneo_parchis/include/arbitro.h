#ifndef ARBITRO_H
#define ARBITRO_H

#include "tipos.h"

/*
 * Ejecuta la lógica principal del árbitro.
 */
void ejecutar_arbitro(EstadoJuego *estado);

/*
 * Ejecuta la planificación Round Robin de los turnos.
 */
void ejecutar_round_robin(EstadoJuego *estado);

/*
 * Envía el turno al jugador correspondiente.
 */
void enviar_turno_jugador(int id_jugador);

/*
 * Recibe la respuesta del jugador después de jugar su turno.
 */
void recibir_respuesta_jugador(int id_jugador);

/*
 * Verifica si algún jugador ganó.
 * Retorna el identificador del ganador o SIN_GANADOR.
 */
int verificar_ganador(EstadoJuego *estado);

/*
 * Finaliza el juego asignando el ganador.
 */
void finalizar_juego(EstadoJuego *estado, int ganador);

#endif
