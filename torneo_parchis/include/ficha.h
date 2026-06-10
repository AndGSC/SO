#ifndef FICHA_H
#define FICHA_H

#include "tipos.h"

void crear_hilos_fichas(int id_jugador, EstadoJuego *estado);
void *ejecutar_ficha(void *arg);
int ejecutar_turno_fichas(int dado, int *ficha_movida);
void finalizar_hilos_fichas(void);
int mover_ficha(FichaContexto *ctx, int dado);

#endif
