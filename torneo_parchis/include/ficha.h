#ifndef FICHA_H
#define FICHA_H

#include "tipos.h"

/*
 * Crea los 4 hilos ficha de un jugador.
 */
void crear_hilos_fichas(int id_jugador, EstadoJuego *estado);

/*
 * Función principal que ejecuta cada hilo ficha.
 */
void *ejecutar_ficha(void *arg);

/*
 * Mueve una ficha usando el valor del dado.
 * Retorna VERDADERO si se movió, FALSO si no pudo moverse.
 */
int mover_ficha(FichaContexto *ctx, int dado);

#endif
