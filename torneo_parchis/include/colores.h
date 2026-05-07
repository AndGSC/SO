#ifndef COLORES_H
#define COLORES_H

#define ROJO 0
#define VERDE 1
#define AZUL 2
#define AMARILLO 3

/*
 * Devuelve el nombre textual del jugador según su identificador.
 *
 * Ejemplos:
 * "Rojo"
 * "Verde"
 * "Azul"
 * "Amarillo"
 */
const char *nombre_jugador(int id_jugador);

/*
 * Devuelve un símbolo de un solo carácter para representar
 * al jugador en el tablero ASCII.
 *
 * Ejemplos:
 * ROJO     -> 'R'
 * VERDE    -> 'V'
 * AZUL     -> 'A'
 * AMARILLO -> 'M'
 */
char simbolo_jugador(int id_jugador);

#endif
