#ifndef CONFIG_H
#define CONFIG_H


#define NUM_JUGADORES 4


#define NUM_FICHAS 4


#define TAM_TABLERO 40

/*
 * Posiciones especiales.
 * BASE indica que la ficha todavía no ha entrado al tablero.
 * META indica que la ficha ya terminó su recorrido.
 */
#define BASE -1
#define META 100

/*
 * Límite máximo de turnos para evitar ejecuciones infinitas
 * en caso de que ningún jugador gane.
 */
#define MAX_TURNOS 1000

/*
 * Capacidad de las zonas especiales controladas con semáforos binarios.
 *
 * Al usar valor 1, solo una ficha puede entrar a la zona protegida
 * en un momento determinado.
 */
#define CAPACIDAD_META 1
#define CAPACIDAD_PASILLO 1


#define DADO_MIN 1
#define DADO_MAX 6


#define SIN_GANADOR -1
#define JUEGO_ACTIVO 0
#define JUEGO_TERMINADO 1


#define FALSO 0
#define VERDADERO 1

#endif
