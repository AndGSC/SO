#ifndef FICHA_H
#define FICHA_H

#include "tipos.h"

/*
 * Crea los 4 hilos ficha de un jugador.
 * Los hilos permanecen vivos durante toda la partida esperando
 * órdenes de movimiento del hilo principal del proceso.
 */
void crear_hilos_fichas(int id_jugador, EstadoJuego *estado);

/*
 * Función principal que ejecuta cada hilo ficha.
 */
void *ejecutar_ficha(void *arg);

/*
 * Publica el dado del turno y despierta a los 4 hilos ficha para
 * que compitan por realizar el movimiento. Bloquea hasta que todos
 * los hilos respondan.
 *
 * Retorna VERDADERO si alguna ficha se movió y deja en *ficha_movida
 * el índice de la ficha que reclamó el movimiento (-1 si ninguna).
 */
int ejecutar_turno_fichas(int dado, int *ficha_movida);

/*
 * Ordena a los hilos ficha terminar y espera su finalización
 * con pthread_join.
 */
void finalizar_hilos_fichas(void);

/*
 * Mueve una ficha usando el valor del dado.
 * Retorna VERDADERO si se movió, FALSO si no pudo moverse.
 */
int mover_ficha(FichaContexto *ctx, int dado);

#endif
