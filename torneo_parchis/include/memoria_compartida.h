#ifndef MEMORIA_COMPARTIDA_H
#define MEMORIA_COMPARTIDA_H

#include "tipos.h"

/*
 * Esta memoria será visible tanto para el proceso padre como para
 * los procesos hijos con fork().
 *
 * Retorna:
 *   puntero a EstadoJuego si la memoria se creó correctamente.
 * Si ocurre un error, la función debe finalizar el programa.
 */
EstadoJuego *crear_memoria_compartida(void);

/*
 * Libera la memoria compartida utilizada por el EstadoJuego.
 * Parámetros:
 *   estado -> puntero a la estructura compartida del juego.
 */
void liberar_memoria_compartida(EstadoJuego *estado);

#endif
