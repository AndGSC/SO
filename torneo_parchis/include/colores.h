#ifndef COLORES_H
#define COLORES_H

#define ROJO 0
#define VERDE 1
#define AZUL 2
#define AMARILLO 3

/*
 * Código ANSI para restaurar el color normal de la terminal.
 */
#define COLOR_RESET "\033[0m"

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

/*
 * Devuelve el código de color ANSI del jugador para la salida
 * del tablero en la terminal.
 */
const char *color_jugador(int id_jugador);

#endif
