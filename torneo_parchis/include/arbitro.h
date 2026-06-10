#ifndef ARBITRO_H
#define ARBITRO_H

#include "tipos.h"

void ejecutar_arbitro(EstadoJuego *estado);
void ejecutar_round_robin(EstadoJuego *estado);
void enviar_turno_jugador(int id_jugador);
void recibir_respuesta_jugador(int id_jugador);
int verificar_ganador(EstadoJuego *estado);
void finalizar_juego(EstadoJuego *estado, int ganador);

#endif
