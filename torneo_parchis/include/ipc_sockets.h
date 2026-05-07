#ifndef IPC_SOCKETS_H
#define IPC_SOCKETS_H

#include "tipos.h"

#define PUERTO_BASE 5000
#define TAM_BUFFER_SOCKET 128

/*
 * Inicializa la estructura de sockets del árbitro.
 */
void inicializar_sockets_arbitro(void);

/*
 * Inicializa la conexión de socket de un jugador.
 */
void inicializar_socket_jugador(int id_jugador);

/*
 * Envía el turno desde el árbitro hacia un jugador.
 */
void enviar_turno_socket(int id_jugador);

/*
 * El jugador espera recibir su turno.
 */
void esperar_turno_socket(int id_jugador);

/*
 * El jugador envía una respuesta al árbitro.
 */
void enviar_respuesta_socket(int id_jugador, const char *mensaje);

/*
 * El árbitro recibe la respuesta de un jugador.
 */
void recibir_respuesta_socket(int id_jugador);

/*
 * Cierra los sockets usados por el árbitro y los jugadores.
 */
void cerrar_sockets(void);

#endif
